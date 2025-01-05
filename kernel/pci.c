#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <pci.h>
#include <unios/memory.h>
#include <unios/proc.h>
#include <unios/protect.h>
#include <arch/x86.h>
#include <unios/tracing.h>
#include <unios/serial.h>

#define DEBUG_PCI

pci_dev_t pci_devs[NR_PCI_DEV];

/*!
 * \brief read bar
 *
 * \attention
 * This function is used internally by the pci library
 * and should not be used by other modules
 *
 * \param bus bus number
 * \param device device number
 * \param function function number
 * \param bar_id bar id
 *
 * \return bar
 */
static uint32_t pci_internal_read_bar(uint8_t bus, uint8_t device, uint8_t function,
                                 uint8_t bar_id) {
    uint32_t header_type = pci_read(bus, device, function, 0x0e) & 0x7f;
    uint8_t max_bars = 6 - (header_type * 4);
    if (bar_id >= max_bars) return 0;
    uint32_t bar_val = pci_read(bus, device, function, 0x10 + 4 * bar_id);
    return bar_val;
}

static bar_t pci_get_bar(uint8_t bus, uint8_t device, uint8_t function,
                          uint8_t bar_id) {
    bar_t result;

    uint32_t bar_val = pci_internal_read_bar(bus, device, function, bar_id);
    result.type = (bar_val & 0x1) ? INPUT_OUTPUT : MEMORY_MAPPED;

    if (result.type == MEMORY_MAPPED) {
    } else {
        result.address = (uint32_t)((bar_val >> 2) << 2);
        result.prefetchable = 0;
    }
    return result;
}

uint32_t pci_read(uint16_t bus, uint16_t device, uint16_t function,
                  uint32_t offset) {
    uint32_t id = (0x1 << 31) | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
                  ((function & 0x07) << 8) | (offset & 0xFC);
    outl(0xCF8, id);
    uint32_t tmp = (uint32_t)(inl(0xCFC) >> (8 * (offset % 4)));
    return tmp;
}

void pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset,
               uint32_t data) {
    uint32_t bus32 = bus;
    uint32_t device32 = device;
    uint16_t function16 = function;
    uint32_t address = (1 << 31) | (bus32 << 16) | (device32 << 11) |
                       (function16 << 8) | (offset & 0xFC);
    outl(0xCF8, address);
    outl(0xCFC, data);
}

static int dev_type_by_class(pci_dev_t* dd) {
    switch (dd->class_id) {
        case 0x1:
            switch (dd->subclass_id) {
                case 0x1:
                case 0x3:
                case 0x4:
                    return DEVICE_BUS_CONTROLLER;
                default:
                    return DEVICE_STORAGE;
            }
        case 0x2:
            return DEVICE_NETWORK;
        case 0x3:
            return DEVICE_DISPLAY;
        case 0x6:
            return DEVICE_BRIDGE;
        default:
#ifdef DEBUG_PCI
            kinfo("PCI: DEVICE_UNKNOWN: Class %d subclass %d", dd->class_id,
                    dd->subclass_id);
#endif
            return DEVICE_UNKNOWN;
    }
}

static char pci_has_device_functions(uint8_t bus, uint8_t device) {
    return pci_read(bus, device, 0, 0x0e) & (1 << 7);
}

int pci_find_devices() {
#ifdef DEBUG_PCI
    kinfo("PCI: scanning\n");

#endif
    uint8_t bus, device, function;
    int count = 0;
    for (bus = 0; bus < 8; bus++) {
        for (device = 0; device < 32; device++) {
            uint8_t functions_count =
                pci_has_device_functions(bus, device) == 0 ? 8 : 1;
            for (function = 0; function < functions_count; function++) {
                pci_dev_t dev =
                    pci_get_device_desriptor(bus, device, function);
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xffff) {
                    continue;
                }

                for (uint8_t bar_id = 0; bar_id < 6; bar_id++) {
                    bar_t bar = pci_get_bar(bus, device, function, bar_id);
                    if (bar.address && (bar.type == INPUT_OUTPUT)) {
                        dev.port_base = (uint32_t)bar.address;
                    }
                }
                dev.type = dev_type_by_class(&dev);
                pci_devs[count++] = dev;
#ifdef DEBUG_PCI
                kinfo("PCI: Vendor %x, devID %x, class %x subclass %x\n",
                        dev.vendor_id, dev.device_id, dev.class_id,
                        dev.subclass_id);
#endif
            }
        }
    }

    return 0;
}

pci_dev_t pci_get_device_desriptor(uint8_t bus, uint8_t device,
                                           uint8_t function) {
    pci_dev_t new_device = {0};

    new_device.bus = bus;
    new_device.device = device;
    new_device.function = function;

    new_device.vendor_id = pci_read(bus, device, function, 0x00);
    new_device.device_id = pci_read(bus, device, function, 0x02);

    new_device.class_id = pci_read(bus, device, function, 0x0b);
    new_device.subclass_id = pci_read(bus, device, function, 0x0a);
    new_device.interface_id = pci_read(bus, device, function, 0x09);
    new_device.revision_id = pci_read(bus, device, function, 0x08);

    new_device.interrupt = pci_read(bus, device, function, 0x3c);

    return new_device;
}

uint32_t pci_read_bar(pci_dev_t* pci_dev, int bar_id) {
    return pci_internal_read_bar(pci_dev->bus, pci_dev->device, pci_dev->function,
                            bar_id);
}

void init_pci() { pci_find_devices(); }

pci_dev_t* get_pci_bga()
{
    kinfo("probing for display device\n");
    for (int i = 0; i < NR_PCI_DEV; ++ i) {
        if (pci_devs[i].type == DEVICE_DISPLAY) {
            kinfo("display device found:\n");
            kinfo("vendor: %x, device: %x\n", pci_devs[i].vendor_id, pci_devs[i].device_id);
            return &pci_devs[i];
        }
    }
    kwarn("display device not found\n");
    return NULL;
}

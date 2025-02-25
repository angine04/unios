#pragma once

#include <lib/stdint.h>

typedef struct {
    char     prefetchable;
    uint32_t address;
    uint8_t  type;
} bar_t;

#define INPUT_OUTPUT  0
#define MEMORY_MAPPED 1

struct device_desc_pci {
    uint8_t  bus;
    uint8_t  device;
    uint8_t  function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t  class_id;
    uint8_t  subclass_id;
    uint8_t  interface_id;
    uint8_t  revision_id;
    uint32_t interrupt;
    uint32_t port_base;
    uint32_t type;
};
typedef struct device_desc_pci pci_dev_t;

enum DEVICES_TYPE {
    DEVICE_STORAGE        = (1 << 0),
    DEVICE_SOUND          = (1 << 1),
    DEVICE_INPUT_SYSTEMS  = (1 << 2),
    DEVICE_NETWORK        = (1 << 3),
    DEVICE_DISPLAY        = (1 << 4),
    DEVICE_BUS_CONTROLLER = (1 << 5),
    DEVICE_BRIDGE         = (1 << 6),
    DEVICE_CHAR           = (1 << 7),
    DEVICE_RTC            = (1 << 8),
    DEVICE_UNKNOWN        = (1 << 9),
};

/*!
 * \brief read data from pci configuration space
 *
 * \param bus bus number
 * \param device device number
 * \param function function number
 * \param offset offset
 *
 * \return data read from pci
 */
uint32_t
    pci_read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset);

/*!
 * \brief write data to pci configuration space
 *
 * \param bus bus number
 * \param device device number
 * \param function function number
 * \param offset offset
 * \param data data to write
 */
void pci_write(
    uint8_t  bus,
    uint8_t  device,
    uint8_t  function,
    uint8_t  offset,
    uint32_t data);

/*!
 * \brief get device descriptor
 *
 * \param bus bus number
 * \param device device number
 * \param function function number
 *
 * \return device descriptor
 */
pci_dev_t
    pci_get_device_desriptor(uint8_t bus, uint8_t device, uint8_t function);

/*!
 * \brief get bga device
 *
 * \return bga device
 */
pci_dev_t* get_pci_bga();

/*!
 * \brief read bar
 *
 * \param pci_dev pci device
 * \param bar_id bar id
 *
 * \return bar
 */
uint32_t pci_read_bar(pci_dev_t* pci_dev, int bar_id);

/*!
 * \brief init pci
 */
void init_pci();

#define NR_PCI_DEV 8

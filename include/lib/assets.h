typedef struct resource_s {
    pixel_t *buf;
    int width;
    int height;
    bool has_alpha;
} resource_t;

enum {
     RESOURCE_ICON_UNIOS = 0,
     RESOURCE_TEST,
};

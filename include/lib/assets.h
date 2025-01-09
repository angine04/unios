typedef struct image_s {
    pixel_t *buf;
    int width;
    int height;
    bool has_alpha;
} image_t;

enum {
     RESOURCE_ICON_UNIOS = 0,
     RESOURCE_IMAGE_WALLPAPER,
     RESOURCE_IMAGE_TEST,
     RESOURCE_ICON_CURSOR,
     RESOURCE_IMAGE_LETTERS,
};

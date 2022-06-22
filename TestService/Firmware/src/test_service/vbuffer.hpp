/*#pragma once
#include <stdint.h>
#include <vector>
#include <string>

namespace tns {

    struct position {
        uint8_t x;
        uint8_t y;
    };
    
    struct size {
        uint8_t w;
        uint8_t h;
    };

    struct rect {
        uint8_t x;
        uint8_t y;
        uint8_t w;
        uint8_t h;

        size get_size();
        position get_pos();

        void set_size(uint8_t w, uint8_t h);  
        void set_size(size size);
        void resize(int dw, int dh);

        void set_pos(uint8_t x, uint8_t y);
        void set_pos(position pos);
        void move(int dx, int dy);

    };

    class vbuffer {
        public:
            const int width = 53;
            const int height = 25;

            void update();
            void fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ch, uint8_t attr);
            voud fill_rect_attr()

        private:
            uint8_t buffer[width*height*2];
    }
}
*/

#include "BGFXCallback.h"
#include <bx/bx.h>
#include <bx/string.h>
#include <bx/platform.h>
#include <bx/debug.h>
#include <stdarg.h>
#include <bgfx/bgfx.h>

namespace Babylon
{
    std::vector<uint8_t> BGFXCallback::screenShotBitmap;

    void BGFXCallback::trace(const char* _filePath, uint16_t _line, const char* _format, ...)
    {
        va_list argList;
        va_start(argList, _format);

        traceVargs(_filePath, _line, _format, argList);

        va_end(argList);
    }

    void BGFXCallback::fatal(const char* filePath, uint16_t line, bgfx::Fatal::Enum code, const char* str)
    {
        if (bgfx::Fatal::DebugCheck == code)
        {
            bx::debugBreak();
        }
        else
        {
            trace(filePath, line, "BGFX 0x%08x: %s\n", code, str);
            BX_UNUSED(code, str);
            abort();
        }
    }

    void BGFXCallback::traceVargs(const char* filePath, uint16_t line, const char* format, va_list argList)
    {
        char temp[2048];
        char* out = temp;
        va_list argListCopy;
        va_copy(argListCopy, argList);
        int32_t len = bx::snprintf(out, sizeof(temp), "%s (%d): ", filePath, line);
        int32_t total = len + bx::vsnprintf(out + len, sizeof(temp) - len, format, argListCopy);
        va_end(argListCopy);
        if ((int32_t)sizeof(temp) < total)
        {
            out = (char*)alloca(total + 1);
            bx::memCopy(out, temp, len);
            bx::vsnprintf(out + len, total - len, format, argList);
        }
        out[total] = '\0';
        bx::debugOutput(out);
    }
    
    void BGFXCallback::profilerBegin(const char* name, uint32_t abgr, const char* filePath, uint16_t line) 
    {
    }

    void BGFXCallback::profilerBeginLiteral(const char* name, uint32_t abgr, const char* filePath, uint16_t line) 
    {
    }

    void BGFXCallback::profilerEnd() 
    {
    }
    
    uint32_t BGFXCallback::cacheReadSize(uint64_t id) 
    { 
        return 0; 
    }
    
    bool BGFXCallback::cacheRead(uint64_t id, void* data, uint32_t size) 
    { 
        return false; 
    }
    
    void BGFXCallback::cacheWrite(uint64_t id, const void* data, uint32_t size) 
    {
    }

    void BGFXCallback::screenShot(const char* filePath, uint32_t width, uint32_t height, uint32_t pitch, const void* data, uint32_t size, bool yflip)
    {
        screenShotBitmap.resize(width * height * 4);
        for (uint32_t y = 0; y < height; y++)
        {
            const uint8_t *ptr = static_cast<const uint8_t*>(data) + (yflip ? (height - y - 1) : y) * pitch;
            memcpy(&screenShotBitmap[y * width * 4], ptr, width * 4);
        }
    }

    void BGFXCallback::captureBegin(uint32_t width, uint32_t height, uint32_t pitch, bgfx::TextureFormat::Enum format, bool yflip)
    {
    }

    void BGFXCallback::captureEnd()
    {
    }

    void BGFXCallback::captureFrame(const void* _data, uint32_t _size)
    {
    }
}

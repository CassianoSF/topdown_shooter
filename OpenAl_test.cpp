#include <AL/al.h>
#include <AL/alc.h>


ALCdevice *device;

device = alcOpenDevice(NULL);
if (!device)
        // handle errors
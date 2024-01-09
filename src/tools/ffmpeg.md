# ffmpeg (1)

## screen capture specific window (x11)

Following snippet allows to select a window which is then captured.
```bash
#!/bin/bash

echo "Click on window to record .."
# Extract window size and x,y offset.
video_args=$(xwininfo \
    | awk '/Absolute upper-left X:/ { xoff = $4 }
           /Absolute upper-left Y:/ { yoff=$4 }
           /Width:/ { if ($2 % 2 == 1) { width=$2-1; } else { width=$2; } }
           /Height:/ { if ($2 % 2 == 1) { height=$2-1; } else { height=$2; } }
           END { printf "-video_size %dx%d -i :0.0+%d,%d", width, height, xoff, yoff  }')

ffmpeg -framerate 25 -f x11grab $video_args -pix_fmt yuv420p $@ output.mp4
```
> Use `yuv420p` pixel format if video is played on the web
> ([ref](https://stackoverflow.com/questions/32829514/which-pixel-format-for-web-mp4-video))

The input `-i 0,0+xoff,yoff` means to capture `$DISPLAY=0.0` starting at the
coordinate `(xoff, yoff)`, which is the left-upper corner, and the size of the
capture is defined by the `-video_size` argument.


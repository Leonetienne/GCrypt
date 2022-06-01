#!zsh

echo "Make sure to have run all visualization executables in ../build/ (cmake -B build)!"
echo "These generate the base images!"

# Copy all images over, but as pngs, and a bit larger
find ../build/ -maxdepth 1 -type f -name '*.bmp' |\
    xargs -I {}\
      convert "{}"\
        -filter box\
        -resize 256x\
        "{}.png"

mv ../build/*.png .

# Create a few gifs

# Singleblock diffusion
convert -delay 10 -loop 0 -dispose previous \
  "visualize-singleblock-diffusion-input.bmp.png"\
  "visualize-singleblock-diffusion-input-flip.bmp.png"\
  "visualize-singleblock-diffusion-input.gif"

convert -delay 10 -loop 0 -dispose previous \
  "visualize-singleblock-diffusion-output.bmp.png"\
  "visualize-singleblock-diffusion-output-flip.bmp.png"\
  "visualize-singleblock-diffusion-output.gif"


# Multiblock diffusion
convert -delay 10 -loop 0 -dispose previous \
  "visualize-multiblock-diffusion-input.bmp.png"\
  "visualize-multiblock-diffusion-input-flip.bmp.png"\
  "visualize-multiblock-diffusion-input.gif"

convert -delay 10 -loop 0 -dispose previous \
  "visualize-multiblock-diffusion-output.bmp.png"\
  "visualize-multiblock-diffusion-output-flip.bmp.png"\
  "visualize-multiblock-diffusion-output.gif"

# Extreme input diffusion
convert -delay 10 -loop 0 -dispose previous \
  "visualize-extreme-input-diffusion-input.bmp.png"\
  "visualize-extreme-input-diffusion-input-flip.bmp.png"\
  "visualize-extreme-input-diffusion-input.gif"

convert -delay 10 -loop 0 -dispose previous \
  "visualize-extreme-input-diffusion-output.bmp.png"\
  "visualize-extreme-input-diffusion-output-flip.bmp.png"\
  "visualize-extreme-input-diffusion-output.gif"


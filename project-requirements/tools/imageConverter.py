from PIL import Image

def png_to_c_array(image_path):
    image = Image.open(image_path)
    
    # Convert the image to RGB mode
    image = image.convert('RGB')
    
    pixels = list(image.getdata())
    
    # Convert RGB values to hex
    hex_values = ['0x{:02x}{:02x}{:02x}'.format(r, g, b) for r, g, b in pixels]
    
    # Split the hex values into lines of 16 values each for C array formatting
    lines = [hex_values[i:i+16] for i in range(0, len(hex_values), 16)]
    
    # Format the lines into C array syntax
    c_array = ',\n'.join([', '.join(line) for line in lines])
    
    return c_array

if __name__ == "__main__":
    image_path = "image.png" # Path to 128x128 PNG image
    c_array = png_to_c_array(image_path)
    
    with open("image_array.hpp", "w") as file: # Output file with image C array
        file.write("const unsigned int image_array[128 * 128] = {\n")
        file.write(c_array)
        file.write("\n};")
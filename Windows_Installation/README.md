# Installing OpenGL and Codeblocks on Windows

## Step 1: Install Codeblocks and Freeglut
<ul>
  <li> Download Codeblocks from: https://sourceforge.net/projects/codeblocks/files/Binaries/17.12/Windows/codeblocks-17.12mingw-setup.exe/download</li>
  <li>Download Freeglut (MinGW) from: https://www.transmissionzero.co.uk/software/freeglut-devel/</li>
  <li>Extract the .zip file corresponding to freeglut</li>
</ul>

## Step 2: Copy-Paste files
Navigate to the following directories in the extracted .zip file w.r.t Freeglut:

### freeglut/include/GL
Copy all .h files and paste it to C:/Program Files (x86)/Codeblocks/MinGW/include/GL/

### freeglut/bin
Copy the .dll file and paste it to C:/Windows/SysWOW64/ (for 64-bit) or C:/Windows/System32/ (for 32-bit)

### freeglut/lib
Copy all .a files and paste it to C:/Program Files (x86)/Codeblocks/MinGW/lib/

## Step 3: Edit files (in Administrator mode)
Navigate to the following folders in C:/Program Files (x86)/Codeblocks/share/Codeblocks/:

### templates/glut.cbp
Replace all occurrences of 'glut32' to 'freeglut'

### templates/wizard/glut/wizard.script
Replace all occurrences of 'glut32' to 'freeglut'

## Step 4: Check if OpenGL is correctly installed
<ul>
  <li>Open Codeblocks > New > Project > GLUT Project</li>
  <li>Set the GLUT Location to C:/Program Files (x86)/Codeblocks/MinGW/</li>
  <li>Run the default code. It should execute without errors</li>
</ul>

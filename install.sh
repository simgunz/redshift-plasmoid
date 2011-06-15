##version 0.2
version="0.2"

mkdir install
echo "Preparing..."
for file in $(ls | grep -v ^install); do cp -r $file install/; done
echo "Packaging..."
zip -r plasma_flux-$version.plasmoid install
echo "Installing..."
plasmapkg -i plasma_flux-$version.plasmoid
echo "Cleaning..."
rm -rf install

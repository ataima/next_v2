export STAGING_DIR=./install_dir
ninja -C build/$1  all install

if [ $? -eq 0 ]
then
       sync	
	tree ./install_dir
fi
TEMPLATE      = subdirs

SUBDIRS = \
bmpImage\project\qt\image.pro\
n2draw\project\qt\n2draw.pro\
tests\project\qt\tests.pro \
meLadder\project\qt\meLadder.pro



n2draw.depends=image
meLadder.depends=n2draw
test.depend=meladder

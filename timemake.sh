#!/bin/sh
# make all
# don't use more than one process
LOG=/tmp/timemake1.txt

function build()
{
    local BUILD_PROCS
    BUILD_PROCS=$1
    make propper >>${LOG}
    ccache -z -C
    sync
    sleep 1
    time make -j ${BUILD_PROCS} 2>&1 >>${LOG}
    echo "-----------------------------------" >>${LOG}
}

# echo "localhost/1" >/etc/distcc/hosts
# build(1)

# echo "localhost/2" >/etc/distcc/hosts
# build(2)

# echo "localhost/1"         >/etc/distcc/hosts
# echo "moon.linux.bogus/1" >>/etc/distcc/hosts
# echo "localhost/1, moon/1"
# build 2
# build 2
# 
# echo "localhost/1"             >/etc/distcc/hosts
# echo "moon.linux.bogus/1,lzo" >>/etc/distcc/hosts
# echo "localhost/1, moon/1,lzo"
# build 2
# build 2

# echo "localhost/2"         >/etc/distcc/hosts
# echo "moon.linux.bogus/2" >>/etc/distcc/hosts
# echo "localhost/2, moon/2"
# build 4
# build 4
# 
# echo "localhost/2"            >/etc/distcc/hosts
# echo "moon.linux.bogus/2,lzo" >>/etc/distcc/hosts
# echo "localhost/2, moon/2,lzo"
# build 4
# build 4
# 
# echo "localhost/2"         >/etc/distcc/hosts
# echo "moon.linux.bogus/4,lzo" >>/etc/distcc/hosts
# echo "localhost/2, moon/4,lzo"
# build 6
# build 6

# echo "monster,lzo" >/etc/distcc/hosts
# echo "moon.linux.bogus,lzo" >>/etc/distcc/hosts
# echo "localhost"         >>/etc/distcc/hosts
# echo "monster,lzo, moon,lzo, localhost 6 parallel builds"
# build 6
# build 6
# build 6
# 
# echo "monster,lzo" >/etc/distcc/hosts
# echo "moon.linux.bogus,lzo" >>/etc/distcc/hosts
# echo "localhost"         >>/etc/distcc/hosts
# echo "monster,lzo, moon,lzo, localhost 8 parallel builds"
# build 8
# build 8
# build 8

echo "monster,lzo" >/etc/distcc/hosts
echo "monster,lzo 6 parallel builds"
build 6
build 6
build 6


# echo "localhost"         >/etc/distcc/hosts
# echo "moon.linux.bogus,lzo" >>/etc/distcc/hosts
# echo "monster,lzo" >>/etc/distcc/hosts
# echo "localhost, moon,lzo, monster,lzo 6 parallel builds"
# build 6
# build 6

# echo "-----------------------------------" >>${LOG}
# echo "localhost/2, moon.linux.bogus/4" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# time make -j6 2>&1 >>${LOG}

# echo "---------- ---------"
# echo "localhost/2"            >/etc/distcc/hosts
# echo "moon.linux.bogus/2,lzo"    >>/etc/distcc/hosts
# echo "monster.linux.bogus/2,lzo" >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "localhost/2, moon.linux.bogus/2, monster.linux.bogus/2" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# time make -j6 2>&1 >>${LOG}
# 
# echo "---------- ---------"
# echo "localhost/1"            >/etc/distcc/hosts
# echo "moon.linux.bogus/1,lzo"    >>/etc/distcc/hosts
# echo "monster.linux.bogus/4,lzo" >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "localhost/1, moon.linux.bogus/1, monster.linux.bogus/4" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# time make -j6 2>&1 >>${LOG}
# 
# echo "---------- ---------"
# echo "monster.linux.bogus/2,lzo" >/etc/distcc/hosts
# echo "localhost/2"          >>/etc/distcc/hosts
# echo "moon.linux.bogus/2,lzo"   >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "monster.linux.bogus/2, localhost/2, moon.linux.bogus/2" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# time make -j6 2>&1 >>${LOG}

# echo "---------- ---------"
# echo "monster.linux.bogus/4,lzo" >/etc/distcc/hosts
# echo "localhost/1"         >>/etc/distcc/hosts
# # echo "moon.linux.bogus/2" >>/etc/distcc/hosts
# # echo "monster.linux.bogus/2" >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "monster.linux.bogus/4" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# time make -j5 2>&1 >>${LOG}

# echo "---------- ---------"
# echo "monster.linux.bogus/4,lzo" >/etc/distcc/hosts
# # echo "localhost/2"         >/etc/distcc/hosts
# # echo "moon.linux.bogus/2" >>/etc/distcc/hosts
# # echo "monster.linux.bogus/2" >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "monster.linux.bogus/4" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# sync
# time make -j4 2>&1 >>${LOG}
# 
# echo "---------- ---------"
# echo "monster.linux.bogus/3,lzo" >/etc/distcc/hosts
# # echo "localhost/2"         >/etc/distcc/hosts
# # echo "moon.linux.bogus/2" >>/etc/distcc/hosts
# # echo "monster.linux.bogus/2" >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "monster.linux.bogus/3,lzo" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# sync
# time make -j3 2>&1 >>${LOG}

# echo "---------- ---------"
# echo "monster.linux.bogus/2,lzo" >/etc/distcc/hosts
# # echo "localhost/2"         >/etc/distcc/hosts
# # echo "moon.linux.bogus/2" >>/etc/distcc/hosts
# # echo "monster.linux.bogus/2" >>/etc/distcc/hosts
# echo "-----------------------------------" >>${LOG}
# echo "monster.linux.bogus/2" >>${LOG}
# make propper >>${LOG}
# ccache -z -C
# time make -j2 2>&1 >>${LOG}

# make in ccache
#make propper
#ccache -z -C
#time make -j4 2>&1 >>${LOG}


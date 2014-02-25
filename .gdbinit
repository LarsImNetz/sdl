# set args -f -n MelC.mp3
# -p directory
# -c cursor position
# set args -p /space/local/MpegAudio -c 3 -d -v
# set args -p /alias/Media/MpegAudio/petula-clark-downtown.mp3 -c 3 -d
set args /space/local/private/Media/MpegVideo/Animatrix.avi

break main
# break Directory::fillArray
break mySDL::testPrimitives

# break SimpleController::handleKey_Return
# break PlayListIterator::getCurrentElement
# break PlayListIterator::getSize
# break ViewDirectory::copyBackground
break SimpleController::handleKey_F10

handle SIG32 nostop noprint


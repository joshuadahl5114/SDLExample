# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.simple.Debug:
/Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/Debug/simple:
	/bin/rm -f /Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/Debug/simple


PostBuild.simple.Release:
/Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/Release/simple:
	/bin/rm -f /Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/Release/simple


PostBuild.simple.MinSizeRel:
/Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/MinSizeRel/simple:
	/bin/rm -f /Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/MinSizeRel/simple


PostBuild.simple.RelWithDebInfo:
/Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/RelWithDebInfo/simple:
	/bin/rm -f /Users/joshuadahl/Desktop/Tools/OpenGL\ Book\ Examples/chapter_1/simple/RelWithDebInfo/simple




# For each target create a dummy ruleso the target does not have to exist

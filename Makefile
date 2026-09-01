TARGET      	:=  $(notdir $(CURDIR))
BUILD       	:=  build
LIBBUTANO   	:=  ./butano
PYTHON      	:=  python
SOURCES     	:=  src
INCLUDES 		:= include $(BUILD) 
DATA := 
GRAPHICS 		:= graphics $(BUILD)/fonts
AUDIO       	:=  audio
AUDIOBACKEND	:=  maxmod
AUDIOTOOL		:=  
DMGAUDIO    	:=  dmg_audio
DMGAUDIOBACKEND	:=  default
ROMTITLE    	:=  ROM TITLE
ROMCODE     	:=  SBTP
USERFLAGS   	:=  
USERCXXFLAGS	:=  
USERASFLAGS 	:=  
USERLDFLAGS 	:=  
USERLIBDIRS 	:=  
USERLIBS    	:=  
DEFAULTLIBS 	:=  
STACKTRACE		:=	
USERBUILD   	:=  
EXTTOOL     	:=  
FONTS := graphics/fonts/Unifont
TEXTS := graphics/fontTexts/chars.txt
EXTTOOL := $(PYTHON) butano/tools/butano_fonts_tool.py \
    --build=$(BUILD) \
    --fonts="$(FONTS)" \
    --texts="$(TEXTS)"
#---------------------------------------------------------------------------------------------------------------------
# Export absolute butano path:
#---------------------------------------------------------------------------------------------------------------------
ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

#---------------------------------------------------------------------------------------------------------------------
# Include main makefile:
#---------------------------------------------------------------------------------------------------------------------
include $(LIBBUTANOABS)/butano.mak

find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS} -DTMAKECONF)

include_directories(${PROJECT_SOURCE_DIR}/src/qtparser ${QT_INCLUDE_DIR})

set(headers 
	   QtGui.h
           QtGuiApplication.h
           QtGuiBrowser.h
           QtGuiButton.h
           QtGuiButtonGroup.h
           QtGuiCentralWidget.h
           QtGuiCheckBox.h
           QtGuiComboBox.h
           QtGuiConnect.h
           QtGuiDialog.h
           QtGuiDialogItem.h
           QtGuiDialogItemPrivate.h
           QtGuiDialogItemTypeInt.h
           QtGuiDockWindow.h
           QtGuiLabel.h
           QtGuiLayout.h
           QtGuiLayoutPrivate.h
           QtGuiLineEdit.h
           QtGuiMenu.h
           QtGuiMenuItem.h
           QtGuiMenuSeparator.h
           QtGuiPrivate.h
           QtGuiRadioButton.h
           QtGuiScrollView.h
           QtGuiSignal.h
           QtGuiSlider.h
           QtGuiStatusBar.h
           QtGuiSubMenu.h
           QtGuiTabView.h
           QtGuiToolBar.h
           QtGuiVarModifier.h
           QtGuiWidget.h)

set(sources 
           QtGui.cpp
           QtGuiApplication.cpp
           QtGuiBrowser.cpp
           QtGuiButton.cpp
           QtGuiButtonGroup.cpp
           QtGuiCentralWidget.cpp
           QtGuiCheckBox.cpp
           QtGuiComboBox.cpp
           QtGuiConnect.cpp
           QtGuiDialog.cpp
           QtGuiDialogItem.cpp
           QtGuiDialogItemPrivate.cpp
           QtGuiDialogItemTypeInt.cpp
           QtGuiDockWindow.cpp
           QtGuiLabel.cpp
           QtGuiLayout.cpp
           QtGuiLayoutPrivate.cpp
           QtGuiLineEdit.cpp
           QtGuiMenu.cpp
           QtGuiMenuItem.cpp
           QtGuiMenuSeparator.cpp
           QtGuiObjectTable.cpp
           QtGuiPrivate.cpp
           QtGuiRadioButton.cpp
           QtGuiScrollView.cpp
           QtGuiSignal.cpp
           QtGuiSlider.cpp
           QtGuiStatusBar.cpp
           QtGuiSubMenu.cpp
           QtGuiTabView.cpp
           QtGuiToolBar.cpp
           QtGuiVarModifier.cpp
           QtGuiWidget.cpp)

qt_wrap_cpp(moclib mocsrc ${headers})

add_library(qtgui ${sources} ${mocsrc})

install(TARGETS qtgui DESTINATION lib)
install(FILES ${headers} DESTINATION include)
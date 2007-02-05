<application name="General Data Analyser" label="General Data Analyser - GeoAIDA"
   connect=main.message(QString,int)-statusbar.message(QString)>
<menu>
  <submenu label="&File">
    <item label="New" accel=Ctrl+N >
    <item label="Load Net" accel="Ctrl+L" connect=-main.netLoad()>
    <item label="Save Net" accel="Ctrl+S" connect=-main.netSave()>
    <item label="Load Result" connect=-main.resultLoad()>
    <item label="Save Result" connect=-main.resultSave()>
    <item label="Load Project" accel="Ctrl+P" connect=-main.prjLoad()>
    <item label="Save Project" connect=-main.prjLoad()>
    <separator>
    <item label="&Quit" accel=Ctrl+Q connect=-qapp.quit()>
  </submenu>
  <submenu label="&View">
    <item label="Scene Viewer" accel=Ctrl+V connect=-sceneViewerWin.show()>
    <item label="Trash Viewer" accel=Ctrl+T connect=-trashViewerWin.show()>
    <item label="Map Viewer" accel=Ctrl+M connect=-mapViewerWin.show()>
    <item label="Debug Labelimage Viewer" accel=Ctrl+D connect=-debugViewerWin.show()>
    <item label="Start" connect=-main.start()>
    <item label="Attribute Type"  connect=-attribType.show()>
    <separator>
    <item label="Task Viewer"
          connect=valueChanged(bool)-valueSet
          connect=valueChanged(bool)-taskViewerWin.show>
  </submenu>
</menu>
<toolbar>
  <button pixmap=filenew.xpm tip="New File">
  <button pixmap=fileopen.xpm tip="Load Net" connect=-main.fileLoad()>
  <button pixmap=filesave.xpm tip="Save Net" connect=-main.fileSave()>
  <button label="Test" connect=-main.test() tip="i don't know">
  <button pixmap=start.png connect=-main.analyze() connect=-sceneViewerWin.show() connect=main.readyToAna(bool)-enable(bool) tip="start the analysis">
  <button pixmap=cleanup.xpm connect=-main.cleanUp() tip="Clean up: remove temporary files">
#  button label="Analyze" connect=-main.analyze() connect=-sceneViewerWin.show() connect=main.readyToAna(bool)-enable(bool) tip="start the analysis">
  <label label="<b>Strategy:</b>">
  <combobox val=0 connect=-main.opMode(int)  connect=main.readyToAna(bool)-enable(bool) tip="select mode for the analysis">
    <item id=0 label="      asynchronous     ">
    <item id=1 label="asynchronous & stepwise">
    <item id=2 label="        synchron       ">
    <item id=3 label="  synchron & stepwise  ">
  </combobox>  
  <button label="Step" connect=-main.step() connect=main.interactive(bool)-enable(bool) tip="analysis: next step">
  <button label="Continue" connect=-main.proceed()  connect=main.interactive(bool)-enable(bool) tip="continue the analysis">
  <button label="Stop" connect=-main.stop()  connect=main.interactive(bool)-enable(bool) tip="stop the analysis">
</toolbar>

<centralwidget layout=hbox minheight=512 minwidth=512>
<widget colstretch=4 name=netEditor
        connect=itemChanged(TreeItem*,GNode*)-propertyEditor.setNode(TreeItem*,GNode*)>
	      connect=treeChanged()-mapViewer.generateMapImage()
<widget colstretch=1 name=propertyEditor>
</centralwidget>

<statusbar name=statusbar>
</statusbar>

*************************** Map Viewer ****************************
<dialog name=mapViewerWin label="Map viewer" layout=vbox>
  <scrollview>
    <widget name=mapViewer connect=selected(GNode*)-sceneViewer.setCurrentNode(GNode*)>
  </scrollview>
  <label label="Info" connect=mapViewer.info(QString)-valueSet(QString)>
  <slider name=alpha dir=horizontal type=int minval=0 maxval=100 val=50 connect=-mapViewer.setAlpha(int)>
  <layout layout=hbox>
    <button label="Save map" connect=-main.mapSave()>
    <button label="Generate map" connect=-mapViewer.generateMapImage()>
    <button toggle label="Random Colors" connect=-mapViewer.randomColor(bool)>
    <button label="+" connect=-mapViewer.zoomIn()>
    <button label="-" connect=-mapViewer.zoomOut()>
  </layout>
</dialog>

*************************** Scene Viewer ****************************
<dialog name=sceneViewerWin label="Scene Viewer" layout=hbox>
 	<widget minwidth=200 minheight=400 name=sceneViewer
 	        connect=itemChanged(TreeItem*,GNode*)-sceneAttributes.setNode(TreeItem*,GNode*)
 	        connect=treeChanged()-mapViewer.generateMapImage()
 	        connect=itemChanged(int,GeoImage*)-debugViewer.set(int,GeoImage*)
 	        connect=netEditor.treeChanged()-adaptViewToSNodeTree()>
  <widget  minwidth=200 minheight=400 name=sceneAttributes>
</dialog>

*************************** Task Viewer ****************************
<dockwindow edge=Bottom name=taskViewerWin label="Task Viewer">
  <layout layout=vbox>
   	<widget minwidth=500 minheight=100 name=taskViewer>
   	<layout layout=hbox>
     	<label label=SystemLoad:>
     	<label label=1.0 name=systemLoad connect=taskViewer.systemLoad(double)-valueSet(double)>
    </layout>
  </layout>
</dockwindow>

*************************** Trash Viewer ****************************
<dialog name=trashViewerWin label="Trash Viewer" layout=hbox>
 	<widget minwidth=200 minheight=400 name=trashViewer
 	        connect=itemChanged(TreeItem*,GNode*)-trashAttributes.setNode(TreeItem*,GNode*)
 	        connect=treeChanged()-mapViewer.generateMapImage()
 	        connect=itemChanged(int,GeoImage*)-debugViewer.set(int,GeoImage*)>
  <widget  minwidth=200 minheight=400 name=trashAttributes>
</dialog>

*************************** Debug Viewer ****************************
<dialog name=debugViewerWin label="Debug labelimage viewer" layout=vbox>
  <scrollview>
   	<widget minwidth=200 minheight=400 name=debugViewer>
  </scrollview>
  <layout layout=hbox>
  <button label="+" connect=-debugViewer.zoomIn()>
  <button label="-" connect=-debugViewer.zoomOut()>
  </layout>
 	label label="DebugInfo" connect=debugViewer.info(QString)-valueSet(QString)
</dialog>

*************************** Attribute Dialog ****************************
<dialog name=attribType title="Attribute Type" layout=grid>
  <label x=0 y=0 label="Type of Attribute: ">
	<combobox x=1 y=0 name=aType connect=-propertyEditor.setType(int)>
	  <item id=1 label=String>
	  <item id=2 label=Integer>
	  <item id=3 label=Double>
	  <item id=4 label=Enum>
	  <item id=5 label=Operator>
	</combobox>
	<label x=0 y=1 label="min: ">
	<input x=1 y=1 type=double connect=propertyEditor.typeNO()-disable() connect=propertyEditor.typeDOUBLE()-enable()>
	<label x=0 y=2 label="max: ">
	<input x=1 y=2 type=double connect=propertyEditor.typeNO()-disable() connect=propertyEditor.typeDOUBLE()-enable()>
	<label x=0 y=3 label="min: ">
	<input x=1 y=3 type=int connect=propertyEditor.typeNO()-disable() connect=propertyEditor.typeINT()-enable()>
	<label x=0 y=4 label="max: ">
	<input x=1 y=4 type=int connect=propertyEditor.typeNO()-disable() connect=propertyEditor.typeINT()-enable()>
	<label x=0 y=5 label="options: ">
	<input x=1 y=5 type=string connect=propertyEditor.typeNO()-disable() connect=propertyEditor.typeENUM()-enable()>
	<label x=0 y=6 label="value:">
	<input x=1 y=6 type=string>
</dialog>

*************************** Formula Dialog ****************************
<dialog name=formulaEditorWin title="Formula Editor" layout=grid>
  <widget x=0 y=0 name=formulaEditor minwidth=300 minheight=200>
</dialog>

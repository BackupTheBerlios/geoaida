<application name="Net & Regionfile Viewer" label="Net & Regionfile Viewer"
   connect=main.message(QString,int)-statusbar.message(QString)>
<menu>
  <submenu label="&File">
    <item label="Load &Net" accel="Ctrl+n" connect=-main.loadNet()>
    <item label="Load &Map" accel="Ctrl+m" connect=-main.loadMap()>
    <item label="Load &Data image" accel="Ctrl+d" connect=-main.loadDataImage()>
    <separator>
    <item label="Load &Class Net" accel="Ctrl+c" connect=-main.loadClassNet()>
    <item label="&Remove Class Net" accel="Ctrl+r" connect=-main.clearClassNet()>
    <separator>
    <item label="&Quit" accel=Ctrl+Q connect=-qapp.quit()>
  </submenu>
  <submenu label="&View">
    <item label="Net Viewer" accel=Ctrl+N connect=-netViewerWin.show()>
    <item label="Attribute Viewer" accel=Ctrl+A connect=-attributeViewerWin.show()>
    <item label="Class Viewer" accel=Ctrl+C connect=-classViewerWin.show()>
    <separator>
    <item label="Map->Data" connect=-mapViewer.scaleMap()>
    <item label="Stretch Contrast" connect=-mapViewer.stretchContrast()>
  </submenu>
</menu>
<toolbar>
  <button pixmap=fileopen.xpm tip="Load Net" connect=-main.fileLoad()>
</toolbar>

<centralwidget layout=vbox minheight=512 minwidth=512>
  <scrollview>
    <widget name=mapViewer connect=selected(GNode*)-treeEditor.setCurrentNode(GNode*)>
  </scrollview>
  <label label="Info" connect=mapViewer.info(QString)-valueSet(QString)>
  <slider name=alpha dir=horizontal type=int minval=0 maxval=100 val=50 connect=-mapViewer.setAlpha(int)>
  <layout layout=hbox>
    <button label="Node Colors" connect=-mapViewer.nodeColor()>
    <button label="INode Colors" connect=-mapViewer.iNodeColor()>
    <button label="Random Colors" connect=-mapViewer.randomColor()>
    <button label="Depth Colors" connect=-mapViewer.depthColor()>
    <button label="+" connect=-mapViewer.zoomIn()>
    <button label="-" connect=-mapViewer.zoomOut()>
  </layout>
</centralwidget>

<statusbar name=statusbar>
</statusbar>

*************************** Class Viewer ****************************
<dialog name=classViewerWin label="Class Viewer" layout=vbox>
  <widget colstretch=4 minwidth=400 name=classTree>
</dialog>

*************************** Net Viewer ****************************
<dialog name=netViewerWin label="Net Viewer" layout=vbox>
  <widget colstretch=4 minwidth=400 name=treeEditor
          connect=itemChanged(TreeItem*,GNode*)-propertyEditor.setNode(TreeItem*,GNode*)
          connect=treeChanged()-mapViewer.generateMapImage()>
</dialog>

*************************** Attribute Viewer ****************************
<dialog name=attributeViewerWin label="Scene Viewer" layout=vbox>
  <widget colstretch=1 name=propertyEditor>
</dialog>


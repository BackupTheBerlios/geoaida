<application name=GAD label="GAD Graphical Application Designer" 
             minwidth=256 minheight=256 pixmap=icon.xpm
             x=100 y=100 style=Windows>
<menu>
  <submenu name=file label="&File">
    <item label="&New Workspace" accel=Ctrl+n connect=-gad.newWorkspace()>
    <item label="&Load Workspace" accel=Ctrl+l connect=-gad.loadWorkspace()>
    <item label="&Save Workspace" accel=Ctrl+s connect=-gad.saveWorkspace()>
    <separator>
    <item label="Quit" accel=Ctrl+q connect=-qapp.quit()>
  </submenu>
  <widget name=classmenu>
</menu>
<toolbar>
  <button label="Run">
  <button label="Stop">
</toolbar>

<centralwidget>
  layout layout=hbox>
  label label="Workspace">
  <widget name=centralWidget minwidth=500 minheight=500>
  /layout>
</centralwidget>

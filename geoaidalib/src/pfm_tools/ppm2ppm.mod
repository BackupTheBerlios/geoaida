<MODULE class=PPM name=ppm2ppm
        tip="<p>Scale PPM</p><p>PPM - Portable Pixmap Map</p>"
	cmd="ppm2ppm @input@ @output@ @xdim@ @ydim@">
  <INPUT name=input label="Source PPM File: " tip="PPM Input File">
  <OUTPUT name=output depend=input label="Destination PPM File: " tip="PPM Output File">
  <TEXT name=xdim label="X-Dimension-Val:" tip="<p>Dimension in X-Richtung des neuen bildes</p>">
  <TEXT name=ydim label="Y-Dimension-Val:" tip="<p>Dimension in Y-Richtung des neuen bildes</p>">
</MODULE>


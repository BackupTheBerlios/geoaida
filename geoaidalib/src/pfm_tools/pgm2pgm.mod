<MODULE class=PGM name=pgm2pgm
        tip="<p>Scale PGM</p><p>PGM - Portable Grey Map</p>"
	cmd="pgm2pgm @input@ @output@ @xdim@ @ydim@">
  <INPUT name=input label="Source PGM File: " tip="PGM Input File">
  <OUTPUT name=output depend=input label="Destination PGM File: " tip="PGM Output File">
  <TEXT name=xdim label="X-Dimension-Val:" tip="<p>Dimension in X-Richtung des neuen bildes</p>">
  <TEXT name=ydim label="Y-Dimension-Val:" tip="<p>Dimension in Y-Richtung des neuen bildes</p>">
</MODULE>


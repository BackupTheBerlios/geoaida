<MODULE class=PFM name=pfm2pfm
        tip="<p>Scale PFM</p><p>PFM - Portable Float Map</p>"
	cmd="pfm2pfm @input@ @output@ @xdim@ @ydim@">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <TEXT name=xdim label="X-Dimension-Val:" tip="<p>Dimension in X-Richtung des neuen bildes</p>">
  <TEXT name=ydim label="Y-Dimension-Val:" tip="<p>Dimension in Y-Richtung des neuen bildes</p>">
</MODULE>

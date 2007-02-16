<MODULE class=PLM name=plm2pbm
        tip="<p>Convert PLM to PBM</p><p>PFM - Portable Label Map<br>PBM - Portable Bitmap Map</p>"
	cmd="plm2pbm @input@ @output@">
  <INPUT name=input label="PLM File: " tip="PLM Input File">
  <OUTPUT name=output depend=input label="PBM File: " tip="PBM Output File">
</MODULE>


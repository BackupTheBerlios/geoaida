<MODULE class=PLM name=plmdump
        tip="<p>Convert PLM to RAW</p><p>PFM - Portable Label Map<br>PGM - Portable Grey Map</p>"
	cmd="plmdump @input@ @output@">
  <INPUT name=input label="PLM File: " tip="PLM Input File">
  <OUTPUT name=output depend=input label="PGM File: " tip="PGM Output File">
</MODULE>

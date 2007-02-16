<MODULE class=PLM name=plm2pgm
        tip="<p>Convert PLM to PGM</p><p>PFM - Portable Label Map<br>PGM - Portable Grey Map</p>"
	cmd="plm2pgm @input@ @output@">
  <INPUT name=input label="PLM File: " tip="PLM Input File">
  <OUTPUT name=output depend=input label="PGM File: " tip="PGM Output File">
</MODULE>

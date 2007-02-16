<MODULE class=PGM name=pgm2pfm
        tip="<p>Convert PGM to PFM</p><p>PFM - Portable Float Map<br>PGM - Portable Grey Map</p>"
	cmd="pgm2pfm @input@ @output@">
  <INPUT name=input label="PGM File: " tip="PGM Input File">
  <OUTPUT name=output depend=input label="PFM File: " tip="PFM Output File">
</MODULE>

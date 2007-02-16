<MODULE class=PFM name=pfm2pgm
        tip="<p>Convert PFM to PGM</p><p>PFM - Portable Float Map<br>PGM - Portable Grey Map</p>"
	cmd="pfm2pgm @input@ @output@ @minval@ @maxval@">
  <INPUT name=input label="PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="PGM File: " tip="PGM Output File">
  <TEXT name=minval label="Min-Val:" tip="<p><i>(optional)</i></p><p>Dieser Wert wird auf den Grauwert 1 abgebildet</p>"
  <TEXT name=maxval label="Max-Val:" tip="<p><i>(optional)</i></p><p>Dieser Wert wird auf den Grauwert 255 abgebildet</p>"
</MODULE>


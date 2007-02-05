<MODULE class=GaImage name=scale
        tip="<p>Scale PFM</p><p>PFM - Portable Float Map</p>"
	cmd="gi_scale @input@ @output@ @multiply@ @divisor@">
  <INPUT name=input label="Source PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="Destination PFM File: " tip="PGM Output File">
  <TEXT name=multiply tip="Multiplikator" value=1.0>
  <TEXT name=divisor label="Divisor" value=1.0>
</MODULE>

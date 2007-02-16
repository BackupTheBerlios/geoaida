<MODULE class=PFM name=pfmcutgeoregion
        tip="Cut a geo region in PFM file and write it to a file"
	cmd="pfmcutgeoregion -geo @input@ @output@ @west@ @north@ @east@ @south@">
  <INPUT name=input label="PFM File: " tip="PFM/PGM/PPM Input File">
  <OUTPUT name=output depend=input label="PFM File: " tip="PFM/PGM/PPM Output File">
  <TEXT name=west>
  <TEXT name=north>
  <TEXT name=east>
  <TEXT name=south>
</MODULE>
	

<MODULE class=PGM name=pgmcutregion
        tip="Cut a region in PGM file and write it to a file"
	cmd="pgmcutregion @input@ @output@ @xmin@ @ymin@ @xmax@ @ymax@">
  <INPUT name=input label="PGM File: " tip="PGM Input File">
  <OUTPUT name=output depend=input label="PGM File: " tip="PGM Output File">
  <TEXT name=xmin>
  <TEXT name=ymin>
  <TEXT name=xmax>
  <TEXT name=ymax>
</MODULE>

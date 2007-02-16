<MODULE class=PFM name=pfmcutregion
        tip="Cut a region in PFM file and write it to a file"
	cmd="pfmcutregion @input@ @output@ @xmin@ @ymin@ @xmax@ @ymax@">
  <INPUT name=input label="PFM File: " tip="PFM Input File">
  <OUTPUT name=output depend=input label="PGM File: " tip="PGM Output File">
  <TEXT name=xmin>
  <TEXT name=ymin>
  <TEXT name=xmax>
  <TEXT name=ymax>
</MODULE>
	

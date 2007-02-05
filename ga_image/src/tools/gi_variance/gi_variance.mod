<MODULE class=GaImage name=variance
        tip="<p>Calculate Variance</p>"
	cmd="gi_variance @circular@ @input@ @output@ @outputvalues@ @winsize@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <OUTPUT name=output depend=input>
  <OUTPUT name=outputvalues depend=input>
  <TEXT name=winsize value=3>
  <bool name=circular on=-c off="">
</MODULE>


<MODULE class=GaImage name=gaussian
        tip="<p>Apply Gaussian blur to image</p>"
	cmd="gi_gaussian @input@ @sigma@ @output@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <TEXT name=sigma label="Sigma" tip="<p>Strength of blurring, for reasonably sized images try 1 or 2</p>" cmd="@sigma@">
  <OUTPUT name=output depend=input label="Blurred output: " tip="Resulting image with Gaussian blur applied">
</MODULE>


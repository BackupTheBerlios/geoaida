<MODULE class=GaImage name=canny
        tip="<p>Apply Canny algorithm to image</p>"
	cmd="gi_canny @input@ @canny@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <OUTPUT name=canny depend=input label="Canny output: " tip="Resulting image with Canny algorithm applied">
</MODULE>


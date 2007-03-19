<MODULE class=GaImage name=canny
        tip="<p>Apply Canny algorithm to image</p>"
	cmd="gi_canny @input@ @output@ @lowthreshold@ @highthreshold@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <OUTPUT name=output depend=input label="Canny output: " tip="Resulting image with Canny algorithm applied">
  <TEXT name=lowthreshold label="Low threshold: " tip="<p>Minimal value of a pixel to be considered a part of an edge</p>" cmd="@lowthreshold@" value="0.04">
  <TEXT name=highthreshold label="High threshold: " tip="<p>Minimal value of a pixel to be considered the start of an edge</p>" cmd="@highthreshold@" value="0.05">
</MODULE>

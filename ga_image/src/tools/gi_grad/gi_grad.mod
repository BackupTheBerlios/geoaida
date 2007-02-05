<MODULE class=GaImage name=gradient
        tip="<p>Convert PPM to abs-grad and angle-grad</p>"
	cmd="gi_grad @hsv@ @input@ @abs@ @angle@">
  <INPUT name=input label="Input: " tip="PFM/PGM/PNM/PBM/PPM-Files are supported">
  <OUTPUT name=abs depend=input label="Abs: " tip="Absolute gradient">
  <OUTPUT name=angle depend=input label="Angle: " tip="Angle gradient">
  <bool name=hsv on=-h off="">
</MODULE>


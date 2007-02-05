<MODULE class=GaImage name=denoise
        tip="<p>Remove noise from an image without removing the edges</p>"
	cmd="gi_denoise  @input@ @output@">
  <INPUT name=input label="Source File: " tip="Input File (Any image type)">
  <OUTPUT name=output depend=input label="Destination File: " tip="Will be the same type as the input">
</MODULE>

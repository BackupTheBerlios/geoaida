<MODULE name="MaskPFM" class="PFM Evaluate"
        cmd="pfmmask @input@ @bitmap@ @output@ @mask_val@"
	tip="Close NAN-holes in pfm">
  <INPUT name=input>
  <INPUT name=bitmap>
  <OUTPUT name=output>
  <TEXT name=mask_val value=0>
</MODULE>

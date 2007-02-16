<MODULE name="VariancePFM" class="PFM Evaluate"
        cmd="pfmvariance @input@ @output@ @winsize@ @scalesize@">
  <INPUT name=input>
  <OUTPUT name=output>
  <TEXT name=winsize label="Window Size" tip="size of the window the variance is calculated<br>Should be an odd number<br>">
  <TEXT name=scalesize label"Size of scale" tip="For circular scale in opposition to linear scale e.g. degrees you must provide the size of this scale<br>">
</MODULE>

<MODULE name="SqrtVariancePFM" class="PFM Evaluate"
        cmd="pfmvariance -sqrt @input@ @output@ @winsize@ @scalesize@">
  <INPUT name=input>
  <OUTPUT name=output>
  <TEXT name=winsize label="Window Size" tip="size of the window the variance is calculated<br>Should be an odd number<br>">
  <TEXT name=scalesize label"Size of scale" tip="For circular scale in opposition to linear scale e.g. degrees you must provide the size of this scale<br>">
</MODULE>

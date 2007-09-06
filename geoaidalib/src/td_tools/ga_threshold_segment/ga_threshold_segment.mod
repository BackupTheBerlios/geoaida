<MODULE name="Threshold Segment" class="GeoAIDA"
        cmd="ga_threshold_segment @input@ @labelpic@ @regions@ @minsize@ @maxsize@ @minval@ @maxval@ @class@">
  <INPUT name=input>
  <OUTPUT name=regions depend=input cmd="@regions@" optional>
  <OUTPUT name=labelpic depend=input cmd="@labelpic@">
  <TEXT name=minsize>
  <TEXT name=maxsize>
  <TEXT name=minval>
  <TEXT name=maxval>
  <TEXT name=class optional>
</MODULE>

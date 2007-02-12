<MODULE name="PPMSplitIntoRegions" class="GeoAIDA"
        cmd="ga_ppm_split_into_regions @regions@ @input@ @labelpic@ @minv@ @hlevel@ @slevel@ @vlevel@  @minsize@ @maxsize@">
  <INPUT name=input>
  <OUTPUT name=regions depend=input cmd="-r @regions@" optional>
  <OUTPUT name=labelpic depend=input cmd="@labelpic@">
  <text name=minv>
  <text name=hlevel>
  <text name=slevel>
  <text name=vlevel>
  <TEXT name=minsize>
  <TEXT name=maxsize>
</MODULE>

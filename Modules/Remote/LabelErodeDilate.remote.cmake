itk_fetch_module(LabelErodeDilate
  "Classes performing morphology efficiently on label images.
  Label collisions are consistently handled, and
  operations are efficient (approximately constant time wrt
  structuring element size). Only circular/spherical/hyperspherical
  structuring elements are supported.
  http://www.insight-journal.org/browse/publication/228
  https://hdl.handle.net/10380/3399"
  #UPSTREAM_GIT_REPOSITORY
  #GIT_REPOSITORY ${git_protocol}://github.com/richardbeare/LabelErodeDilate.git
  # Clone is not synchronized correctly with upstream.
  GIT_REPOSITORY ${git_protocol}://github.com/InsightSoftwareConsortium/LabelErodeDilate.git
  GIT_TAG 911949c765f5bfef1f428c07358d5667e226bdc4
  )

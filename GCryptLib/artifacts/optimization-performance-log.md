Old way (400 rounds, no matrix mult): 38.01s
  - 400 was the minimum for good diffusion

New way (10 rounds, with matrix mult): 1.16s
  - 10 rounds now give sufficient diffusion
  - still using bitsets and strings

With new block class (instead of bitsets) (10 rounds): 0.35s
  - still partially using bitsets and strings

With new block class (6 rounds, still good diffusion): 0.21s

With new templated block class (full block and half block): 0.14s
  - finally no more bitsets

With new sbox, reduction, and expansion function: 0.03s

With additional jumbling in feistel rounds: 0.03s

With direct file reading/writing from data blocks: 0.02s


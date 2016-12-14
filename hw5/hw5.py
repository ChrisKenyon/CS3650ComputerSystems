#####################################
#          Part 2                   #
#####################################

# I VOW THESE WILL ALL BE ONE LINE!

'''

   LIST 1

'''

def sum3(nums):
    return sum(nums)

def rotate_left3(nums):
  return nums[1:]+nums[:1]

def max_end3(nums):
  return [max(nums[0],nums[-1])]*3

def make_ends(nums):
  return [nums[0],nums[-1]]

def has23(nums):
  return True if 2 in nums or 3 in nums else False

'''

   LIST 2

'''

def count_evens(nums):
  return len([x for x in nums if x%2==0])

def sum13(nums):
  return sum([nums[i] for i in range(len(nums)) if nums[i] != 13 and (i==0 or nums[i-1] != 13)])]

def big_diff(nums):
  return max(nums)-min(nums)

# ok so this ruins the array inputted but come on this is hilarious
def sum67(nums):
  return sum(nums) - sum([sum([nums.pop(i) for i in range(nums.index(7,nums.index(6)), nums.index(6)-1, -1)]) for x in range(nums.count(6)) if 6 in nums])

def centered_average(nums):
  return (sum(nums)-min(nums)-max(nums))/(len(nums)-2)

def has22(nums):
  return True in [nums[i] == nums[i+1] == 2 for i in range(len(nums)-1)]

'''

   STRING 1

'''

def extra_end(str):
  return str[-2:]*3

def without_end(str):
  return str[1:-1]

'''

   STRING 2

'''

def double_char(str):
  return ''.join([s*2 for s in str])

def count_code(str):
  return [True for i in range(len(str)-3) if str[i:i+2]=='co' and str[i+3]=='e'].count(True)

def count_hi(str):
  return str.count('hi')

def end_other(a, b):
  return a[-len(b):].lower()==b.lower() or b[-len(a):].lower()==a.lower()

def cat_dog(str):
  return str.count('cat')==str.count('dog')

def xyz_there(str):
  return str.count('xyz')-str.count('.xyz')>0


#####################################
#          Part 2                   #
#####################################

def word_count(str):
    return [len(str.splitlines()),sum([len(s.split(' ')) for s in str.splitlines()]),len(str)]

# obviously this is ridiculous but I was not about to make this the only non-oneliner
def my_count(str):
    return [str.count(d) for d in '0123456789 \t\n'] + [len(str) - sum([str.count(d) for d in '0123456789 \t\n'])]


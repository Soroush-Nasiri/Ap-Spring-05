import random
import string

# Configuration
NUM_TEST_CASES = 100
DICT_SIZE = 5000           
QUERIES_PER_CASE = 3       
COMMON_PREFIX = "a"       
MIN_SUFFIX_LEN = 3
MAX_SUFFIX_LEN = 6
EXTRA_LETTERS_PER_QUERY = 12  


EXTRA_LETTERS = "bcdfghjklmnpqrstvwxyz" 

EXTRA_LETTERS = ''.join(ch for ch in EXTRA_LETTERS if ch != COMMON_PREFIX)

ALLOWED_SUFFIX_LETTERS = string.ascii_lowercase

DICT_LETTERS = [ch for ch in string.ascii_lowercase if ch not in EXTRA_LETTERS]

assert COMMON_PREFIX in DICT_LETTERS

def generate_dictionary(n, common_prefix, min_suffix, max_suffix, allowed_letters):
    """Generate a set of n unique words, all starting with common_prefix."""
    words = set()
    # To avoid collisions, we'll generate deterministic random using fixed seed per test case
    while len(words) < n:
        suffix_len = random.randint(min_suffix, max_suffix)
        suffix = ''.join(random.choices(allowed_letters, k=suffix_len))
        word = common_prefix + suffix
        # Keep words reasonably short (max length 10)
        if len(word) <= 10:
            words.add(word)
    # Convert to list and shuffle
    word_list = list(words)
    random.shuffle(word_list)
    return word_list

def generate_query(dictionary_words, num_extra, extra_letters_pool):
    """Generate a query: pick a random dictionary word, add extra distinct letters, shuffle."""
    base_word = random.choice(dictionary_words)
    query_letters = list(base_word)
    # Determine letters already in the word
    present = set(query_letters)
    # Choose extra letters that are not already present
    available_extra = [ch for ch in extra_letters_pool if ch not in present]
    if len(available_extra) < num_extra:
        # If not enough distinct extra letters, allow duplicates? But better to reduce num_extra
        actual_extra = available_extra
    else:
        actual_extra = random.sample(available_extra, num_extra)
    query_letters.extend(actual_extra)
    random.shuffle(query_letters)
    return query_letters

def write_test_case(filepath, dictionary, queries):
    """Write a single test case to file in required format."""
    with open(filepath, 'w') as f:
        # Number of dictionary words
        f.write(f"{len(dictionary)}\n")
        for word in dictionary:
            f.write(f"{word}\n")
        # Number of queries
        f.write(f"{len(queries)}\n")
        for query in queries:
            f.write(' '.join(query) + '\n')

def main():
    # Use a fixed seed for reproducibility
    random.seed(20240502)
    
    for case_num in range(1, NUM_TEST_CASES + 1):
        # Generate dictionary
        dict_words = generate_dictionary(
            n=DICT_SIZE,
            common_prefix=COMMON_PREFIX,
            min_suffix=MIN_SUFFIX_LEN,
            max_suffix=MAX_SUFFIX_LEN,
            allowed_letters=DICT_LETTERS
        )
        
        # Generate queries
        queries = []
        for _ in range(QUERIES_PER_CASE):
            query = generate_query(
                dictionary_words=dict_words,
                num_extra=EXTRA_LETTERS_PER_QUERY,
                extra_letters_pool=EXTRA_LETTERS
            )
            queries.append(query)
        
        # Write to file
        filename = f"input{case_num}.txt"
        write_test_case(filename, dict_words, queries)
        print(f"Generated {filename}")

if __name__ == "__main__":
    main()
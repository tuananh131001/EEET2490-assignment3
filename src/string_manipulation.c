// Generate a function to find the length of the string
int string_length(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    i++;
  }
  return i;
}

// Generate a function to compare two strings
int string_compare(char *str1, char *str2)
{
  // compare length
  if (string_length(str1) != string_length(str2))
    return 0;

  char *p1 = str1; // pointer pointing to first character in str1
  char *p2 = str2; // pointer pointing to first character in str2

  while (*p1 != '\0' && *p2 != '\0')
  {
    if (*p1 != *p2)
      return 0;

    p1++;
    p2++;
  }
  return 1;
}



// Generate a function to covert string to lowercase
void string_to_lowercase(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    if (str[i] >= 'A' && str[i] <= 'Z')
    {
      str[i] = str[i] + 32;
    }
    i++;
  }
}

// Generate a function to remove space at top of string
void string_remove_extra_spaces(char *str)
{
  int i = 0;
  while (str[i] == ' ')
  {
    i++;
  }
  int j = 0;
  while (str[i] != '\0')
  {
    str[j] = str[i];
    i++;
    j++;
  }
  str[j] = '\0';
}

// generate trim trailing string
void trim_trailing_string(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    i++;
  }
  i--;
  while (str[i] == ' ')
  {
    str[i] = '\0';
    i--;
  }
}

// Generate a function to replace multiple spaces by on space
void replace_multi_space_with_single_space(char *str)
{
  char *dest = str; /* Destination to copy to */

  /* While we're not at the end of the string, loop... */
  while (*str != '\0')
  {
    /* Loop while the current character is a space, AND the next
     * character is a space
     */
    while (*str == ' ' && *(str + 1) == ' ')
      str++; /* Just skip to next character */

    /* Copy from the "source" string to the "destination" string,
     * while advancing to the next character in both
     */
    *dest++ = *str++;
  }

  /* Make sure the string is properly terminated */
  *dest = '\0';
}

// Generate a function to convert string to integer
int string_to_integer(char *str)
{
  int i = 0;
  int result = 0;
  while (str[i] != '\0')
  {
    result = result * 10 + (str[i] - '0');
    i++;
  }
  return result;
}

// Generate a function to check string contains string
int string_contains(char *str, char *substr)
{
  int i = 0;
  int j = 0;
  while (str[i] != '\0')
  {
    if (str[i] == substr[j])
    {
      j++;
      if (substr[j] == '\0')
        return 1;
    }
    else
    {
      j = 0;
    }
    i++;
  }
  return 0;
}

// Generate a function return array of a substring from string by " "

void split_string(char arr[10][20], char *str)
{
  int j = 0;
  int ctr = 0;
  for (int i = 0; i <= (string_length(str)); i++)
  {
    // if space or NULL found, assign NULL into newString[ctr]
    if (str[i] == ' ' || str[i] == '\0')
    {
      arr[ctr][j] = '\0';
      ctr++; // for next word
      j = 0; // for next word, init index to 0
    }
    else
    {
      arr[ctr][j] = str[i];
      j++;
    }
  }
}

// Generate a function to calculate the len of array
int array_length(char arr[10][20])
{
  int i = 0;
  while (arr[i][0] != '\0')
  {
    i++;
  }
  return i;
}

// Generate a function convert integer to string
void integer_to_string(int n, char *str)
{
  int i = 0;
  while (n != 0)
  {
    int rem = n % 10;
    str[i] = rem + '0';
    n = n / 10;
    i++;
  }
  str[i] = '\0';
  // reverse the string
  int j = 0;
  int k = string_length(str) - 1;
  while (j < k)
  {
    char temp = str[j];
    str[j] = str[k];
    str[k] = temp;
    j++;
    k--;
  }
}





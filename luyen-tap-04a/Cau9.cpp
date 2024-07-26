string decimal_to_binary(long number)
{
    if (number < 2) return to_string(number);
    string outp = "";
    char c;
    while (number > 0)
    {
        c = '0' + (number % 2);
        outp = c + outp;
        number = number / 2;
    }
    return outp;
}
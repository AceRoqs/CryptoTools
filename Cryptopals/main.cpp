#include "PreCompile.h"
#include <CrappyCrypto/Base64.h>
#include <PortableRuntime/CheckException.h>
#include <PortableRuntime/Unicode.h>
#include <PortableRuntime/Tracing.h>
//#include <memory.h> // memcpy/memset.

// TODO: It would make sense to move this code to a test framework style.
// clang++ -std=c++11 -stdlib=libc++ main.cpp && ./a.out
// TODO: aligned vs unaligned versions of base64.
// TODO: std::transform instead of for loop over array.
// TODO: possible to eliminate more string functions and operate just on uint8?  operator<<(os, vector)?

// TODO: public include path needs to be in a props file
// TODO: OutDir needs to include both Config and Platform


std::vector<uint8_t> Xor_sum_vectors(const std::vector<uint8_t>& input1, const std::vector<uint8_t>& input2)
{
    assert(input2.size() > 0);

    std::vector<uint8_t> output;
    output.reserve(input1.size());

    auto iter1 = input1.cbegin();
    auto iter2 = input2.cbegin();
    while(iter1 != input1.cend())
    {
        if(iter2 == input2.cend())
        {
            iter2 = input2.cbegin();
        }

        output.push_back(*iter1++ ^ *iter2++);
    }

    return output;
}

#if 0
void Xor_sum(const uint8_t* plain, const uint8_t* key, uint8_t* result, size_t count)
{
    for(auto ix = 0u; ix < count; ++ix)
    {
        result[ix] = plain[ix] ^ key[ix];
    }
}
#endif

#if 0
void Xor_string(const uint8_t* text, uint8_t key, uint8_t* result, size_t count)
{
    for(auto ix = 0u; ix < count; ++ix)
    {
        result[ix] = text[ix] ^ key;
    }
}

void Xor_transform(std::vector<uint8_t>* data, uint8_t key)
{
    std::transform(std::begin(*data), std::end(*data), std::begin(*data), [=](uint8_t datum)
    {
        return datum ^ key;
    });
}
#endif

bool Is_printable(uint8_t ch)
{
    if(ch >= 'A' && ch <= 'Z')
        return true;
    if(ch >= 'a' && ch <= 'z')
        return true;
    if(ch == ' ')
        return true;
    if(ch == '\'')
        return true;
#if 0
    if(ch >= '0' && ch <= '9')
        return true;
    switch(ch)
    {
        case '!':
        case '@':
        case '#':
        case '$':
        case '%':
        case '^':
        case '&':
        case '*':
        case '(':
        case ')':
        case '_':
        case '+':
        case '{':
        case '}':
        case '[':
        case ']':
        case '\\':
        case '/':
        case '?':
        case '.':
        case ',':
        case '\'{':
        case '\"':
        case '<':
        case '>':
        case '|':
            return true;
    }
#endif

    return false;
}

uint8_t Value_from_hex_character(char character)
{
    uint8_t nibble;
    if((character >= '0') && (character <= '9'))
    {
        nibble = character - '0';
    }
    else if((character >= 'A') && (character <= 'F'))
    {
        nibble = character - 'A' + 10;
    }

    CHECK_EXCEPTION((character >= 'a') && (character <= 'f'), std::string(1, character) + u8" is not a valid hex value.");

    nibble = character - 'a' + 10;

    return nibble;
}

unsigned int Score(const std::vector<uint8_t>& text)
{
    auto score = 0;

// TODO : why no std:: in front of for_each?  Check LLVM on mac.
    //for_each(text.cbegin(), text.cend(), [&](uint8_t byte)
    //{
    //    score += Is_printable(byte) ? 1 : 0;
    //});

    for(auto iter = text.cbegin(); iter != text.cend(); ++iter)
    {
        //char a = *iter;
        //if(a > 127)
        //{
        //    return 0;
        //}
        score += Is_printable(*iter) ? 5 : -1;
    }

    return std::max(0, score);
}

std::string Hex_string_from_buffer(const std::vector<uint8_t>& buffer)
{
    std::ostringstream os;

    for_each(buffer.cbegin(), buffer.cend(), [&](int data)
    {
        os << std::hex << std::setfill('0') << std::setw(2) << data;
    });

    return os.str();
}

std::vector<uint8_t> Vector_from_hex_string(const std::string& str)
{
    std::vector<uint8_t> vec;

    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        auto byte = Value_from_hex_character(*iter++);
        byte *= 0x10;
        byte += Value_from_hex_character(*iter);

        vec.push_back(byte);
    }

    return vec;
}

// In lieu of initializer list support in VC2012.
// TODO: Remove this.
template<typename Ty>
std::vector<Ty> Initialize_vector_with_array(const Ty* arr, size_t count)
{
    std::vector<Ty> output;
    output.reserve(count);

    for(decltype(count) ix = 0; ix < count; ++ix)
    {
        output.push_back(arr[ix]);
    }

    return output;
}

std::pair<unsigned int, uint8_t> Find_single_byte_xor_key(const std::vector<uint8_t>& buffer1)
{
    auto best_score = 0u;
    uint8_t best_key = 0;

    uint8_t key = 0;
    for(auto ix = 0; ix < 256; ++ix)
    {
        const std::vector<uint8_t> key_vector(&key, &key + 1);
        const auto xor_sum = Xor_sum_vectors(buffer1, key_vector);

        auto score = Score(xor_sum);
        PortableRuntime::dprintf("Key: %d, score: %u\n", ix, score);
        if(score > best_score)
        {
            best_score = score;
            best_key = key;
        }
        ++key;
    }

    PortableRuntime::dprintf("Best key: %u\nBest score: %u\n", static_cast<unsigned int>(best_key), best_score);

    // TODO: why are types necessary here?
    return std::pair<unsigned int, uint8_t>(best_score, best_key);
    //return std::pair(best_score, best_key);
}

std::vector<uint8_t> Vector_from_string(const std::string& str)
{
    std::vector<uint8_t> vec;
    vec.reserve(str.length());

    for_each(str.cbegin(), str.cend(), [&](char character)
    {
        vec.push_back(character);
    });
    return vec;
}

std::string String_from_vector(const std::vector<uint8_t>& vec)
{
    std::string str;

    for(auto iter = vec.cbegin(); iter != vec.cend(); ++iter)
    {
        if((*iter != 0) && (*iter < 128))
        {
            str.push_back(*iter);
        }
        else
        {
            break;
        }
    }

    return str;

}

unsigned int Hamming_distance(const std::vector<uint8_t>& buffer1, const std::vector<uint8_t>& buffer2)
{
    assert(buffer1.size() == buffer2.size());

    auto hamming_distance = 0u;

    auto difference_vector = Xor_sum_vectors(buffer1, buffer2);
    for_each(difference_vector.cbegin(), difference_vector.cend(), [&](uint8_t byte)
    {
        for(auto ii = 0u; ii < 8; ++ii)
        {
            if(byte & 1)
            {
                ++hamming_distance;
            }
            byte >>= 1;
        }
    });

    return hamming_distance;
}

void Challenge1()
{
    static const uint8_t challenge1_buffer[] =
    {
        0x49, 0x27, 0x6d, 0x20, 0x6b, 0x69, 0x6c, 0x6c,
        0x69, 0x6e, 0x67, 0x20, 0x79, 0x6f, 0x75, 0x72,
        0x20, 0x62, 0x72, 0x61, 0x69, 0x6e, 0x20, 0x6c,
        0x69, 0x6b, 0x65, 0x20, 0x61, 0x20, 0x70, 0x6f,
        0x69, 0x73, 0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x20,
        0x6d, 0x75, 0x73, 0x68, 0x72, 0x6f, 0x6f, 0x6d
    };

    static const uint8_t encodings[][65] =
    {
        "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t",
        "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb28=",
        "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hybw=="
    };

    std::wprintf(L"Challenge 1: Convert hex to base64 and back.\n");

    // Validate permutations of unaligned strings.
    for(auto ix = 0u; ix < sizeof(encodings) / sizeof(encodings[0]); ++ix)
    {
        const std::vector<uint8_t> initial_vector(challenge1_buffer, challenge1_buffer + sizeof(challenge1_buffer) - ix);
        const std::vector<uint8_t> expected_encoding(encodings[ix], encodings[ix] + sizeof(encodings[ix]) - 1);

        const auto encode_production = Base64_from_vector(initial_vector);
        const auto decode_production = Vector_from_base64(encode_production);

        std::wprintf(L"Input:  %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(initial_vector)).c_str());
        std::wprintf(L"Encode: %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(encode_production)).c_str());
        std::wprintf(L"Decode: %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(decode_production)).c_str());

        CHECK_EXCEPTION((encode_production == expected_encoding), u8"Unexpected encoded production.");
        CHECK_EXCEPTION((decode_production == initial_vector), u8"Unexpected decoded production.");

        std::wprintf(L"\n");
    }
}

void Challenge2()
{
    static const uint8_t challenge2_buffer1[] =
    {
        0x1c, 0x01, 0x11, 0x00, 0x1f, 0x01, 0x01, 0x00, 0x06,
        0x1a, 0x02, 0x4b, 0x53, 0x53, 0x50, 0x09, 0x18, 0x1c
    };

    static const uint8_t challenge2_buffer2[] =
    {
        0x68, 0x69, 0x74, 0x20, 0x74, 0x68, 0x65, 0x20, 0x62,
        0x75, 0x6c, 0x6c, 0x27, 0x73, 0x20, 0x65, 0x79, 0x65
    };

    static const uint8_t challenge2_result[] =
    {
        0x74, 0x68, 0x65, 0x20, 0x6b, 0x69, 0x64, 0x20, 0x64,
        0x6f, 0x6e, 0x27, 0x74, 0x20, 0x70, 0x6c, 0x61, 0x79
    };

    std::wprintf(L"Challenge 2: Write a function that takes two equal-length buffers and produces their XOR sum.\n");

    const std::vector<uint8_t> buffer1(challenge2_buffer1, challenge2_buffer1 + sizeof(challenge2_buffer1));
    const std::vector<uint8_t> buffer2(challenge2_buffer2, challenge2_buffer2 + sizeof(challenge2_buffer2));
    const std::vector<uint8_t> expected_result(challenge2_result, challenge2_result + sizeof(challenge2_result));

    const auto xor_sum = Xor_sum_vectors(buffer1, buffer2);

    std::wprintf(L"Buffer 1:        %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(buffer1)).c_str());
    std::wprintf(L"Buffer 2:        %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(buffer2)).c_str());
    std::wprintf(L"Expected result: %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(expected_result)).c_str());
    std::wprintf(L"Xor sum:         %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(xor_sum)).c_str());

    CHECK_EXCEPTION(xor_sum == expected_result, u8"Unexpected XOR sum.");

    std::wprintf(L"\n");
}

void Challenge3()
{
    static const uint8_t challenge3_buffer[] =
    {
        0x1b, 0x37, 0x37, 0x33, 0x31, 0x36, 0x3f, 0x78,
        0x15, 0x1b, 0x7f, 0x2b, 0x78, 0x34, 0x31, 0x33,
        0x3d, 0x78, 0x39, 0x78, 0x28, 0x37, 0x2d, 0x36,
        0x3c, 0x78, 0x37, 0x3e, 0x78, 0x3a, 0x39, 0x3b,
        0x37, 0x36
    };

    std::wprintf(L"Challenge 3: Single-character XOR Cipher.\n");

    const std::vector<uint8_t> buffer1(challenge3_buffer, challenge3_buffer + sizeof(challenge3_buffer));
    const auto key = Find_single_byte_xor_key(buffer1);
    const auto best_score = key.first;
    const auto best_key = key.second;

    const std::vector<uint8_t> buffer2(&best_key, &best_key + 1);
    const auto vec = Xor_sum_vectors(buffer1, buffer2);
    const auto str = String_from_vector(vec);

    std::wprintf(L"Test vector:   %s\n", PortableRuntime::utf16_from_utf8(Hex_string_from_buffer(buffer1)).c_str());
    std::wprintf(L"Best score:    %u\n", best_score);
    std::wprintf(L"Best key:      0x%02x\n", best_key);
    std::wprintf(L"Result string: %s\n", PortableRuntime::utf16_from_utf8(str).c_str());

    CHECK_EXCEPTION(str == u8"Cooking MC's like a pound of bacon", u8"Found wrong key.");

    std::wprintf(L"\n");
}

void Challenge4()
{
    std::wprintf(L"Challenge 4: Detect single-character XOR.\n");

    std::ifstream fs("3132713.txt");
    CHECK_EXCEPTION(!fs.fail(), u8"Could not open 3132713.txt.");

    auto best_score = 0u;
    uint8_t best_index = 0;
    std::vector<uint8_t> best_result;

    auto line = 0u;

    while(!fs.eof())
    {
        std::string str;
        std::getline(fs, str);

        if(str.empty())
        {
            break;
        }

        auto vec = Vector_from_hex_string(str);

        uint8_t key = 0;
        for(auto ix = 0; ix < 256; ++ix)
        {
            const auto buffer2 = Initialize_vector_with_array(&key, 1);
            const auto xor_sum = Xor_sum_vectors(vec, buffer2);

            auto score = Score(xor_sum);
//auto a = String_from_vector(xor_sum);
//if(/*score == 24 &&*/ line == 170 && a[0] == 'N' && a[1] == 'o')
//{
//    std::cout << "V1: " << std::dec << line << " : " << score << " : " << String_from_vector(xor_sum) << std::endl;
//    score = Score(xor_sum);
//}


            if(score > best_score)
            {
                best_score = score;
                best_index = key;
                best_result = std::move(xor_sum);

                //std::cout << "V: " << String_from_vector(best_result) << std::endl;
            }
            ++key;
        }

        ++line;
    }

#if 0
    auto best_score = 0u;
    uint8_t best_index = 0;
    uint8_t key = 0;

    const auto buffer1 = Initialize_vector_with_array(challenge3_string, sizeof(challenge3_string));
    for(auto ix = 0; ix < 256; ++ix)
    {
        const auto buffer2 = Initialize_vector_with_array(&key, 1);
        const auto xor_sum = Xor_sum_vectors(buffer1, buffer2);

        auto score = Score(xor_sum);
        if(score > best_score)
        {
            best_score = score;
            best_index = key;
        }
        ++key;
    }

    auto str = String_from_vector(Xor_sum_vectors(buffer1, Initialize_vector_with_array(&best_index, 1)));

    cout << "Test vector:   " << Hex_string_from_buffer(buffer1) << endl;
    cout << "Best key: " << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(best_index) << endl;
    cout << "Result string: " << str << endl;

    if(str == "Cooking MC's like a pound of bacon")
    {
        cout << "Success." << endl;
    }
    else
    {
        cout << "FAILED." << endl;
    }

#endif
    auto str = String_from_vector(best_result);

    std::wprintf(L"Best score:    %u\n", best_score);
    std::wprintf(L"Best key:      %02x\n", static_cast<int>(best_index));
    std::wprintf(L"Result string: %s\n", PortableRuntime::utf16_from_utf8(str).c_str());

    CHECK_EXCEPTION(str == u8"Now that the party is jumping\n", u8"Found wrong key.");

    std::wprintf(L"\n");
}

void Challenge5()
{
    using std::cout;
    using std::endl;

    static constexpr uint8_t challenge5_plain[] = "Burning 'em, if you ain't quick and nimble\n"
                                                  "I go crazy when I hear a cymbal";
    static constexpr uint8_t challenge5_key[] = "ICE";
    static constexpr uint8_t challenge5_cipher[] =
    {
        0x0b, 0x36, 0x37, 0x27, 0x2a, 0x2b, 0x2e, 0x63, 0x62, 0x2c, 0x2e, 0x69, 0x69, 0x2a, 0x23, 0x69,
        0x3a, 0x2a, 0x3c, 0x63, 0x24, 0x20, 0x2d, 0x62, 0x3d, 0x63, 0x34, 0x3c, 0x2a, 0x26, 0x22, 0x63,
        0x24, 0x27, 0x27, 0x65, 0x27, 0x2a, 0x28, 0x2b, 0x2f, 0x20, 0x43, 0x0a, 0x65, 0x2e, 0x2c, 0x65,
        0x2a, 0x31, 0x24, 0x33, 0x3a, 0x65, 0x3e, 0x2b, 0x20, 0x27, 0x63, 0x0c, 0x69, 0x2b, 0x20, 0x28,
        0x31, 0x65, 0x28, 0x63, 0x26, 0x30, 0x2e, 0x27, 0x28, 0x2f
    };

    cout << "Challenge 5: Repeating-key XOR Cipher." << endl;

    const auto plaintext = Initialize_vector_with_array(challenge5_plain, sizeof(challenge5_plain) - 1);
    const auto key = Initialize_vector_with_array(challenge5_key, sizeof(challenge5_key) - 1);
    const auto expected_result = Initialize_vector_with_array(challenge5_cipher, sizeof(challenge5_cipher));

    const auto xor_sum = Xor_sum_vectors(plaintext, key);

    cout << "Buffer 1:        " << Hex_string_from_buffer(plaintext) << endl;
    cout << "Buffer 2:        " << Hex_string_from_buffer(key) << endl;
    cout << "Expected result: " << Hex_string_from_buffer(expected_result) << endl;
    cout << "Xor sum:         " << Hex_string_from_buffer(xor_sum) << endl;

    if(xor_sum == expected_result)
    {
        cout << "Success." << endl;
    }
    else
    {
        cout << "FAILED." << endl;
    }

    cout << endl;
}

void Challenge6()
{
    using std::cout;
    using std::endl;

    cout << "Challenge 6: Break repeating-key XOR" << endl;

#ifndef NDEBUG
    {
        static constexpr uint8_t text1[] = "this is a test";
        static constexpr uint8_t text2[] = "wokka wokka!!!";
        auto vec1 = Initialize_vector_with_array(text1, sizeof(text1) - 1);
        auto vec2 = Initialize_vector_with_array(text2, sizeof(text2) - 1);
        assert(Hamming_distance(vec1, vec2) == 37);
    }
#endif

    std::ifstream fs("3132752.txt");
    if(fs.fail())
    {
        throw std::exception("Fail opening file.");
    }

    std::string base64_string;
    while(!fs.eof())
    {
        std::string str;
        std::getline(fs, str);

        base64_string += str;
    }

    const auto base64_vector = Vector_from_string(base64_string);
    const auto vec = Vector_from_base64(base64_vector);

    auto best_key_size = 2u;
    auto best_normalized_hamming_distance = 8.0f;

    // TODO: break this into a function(s).
    for(auto key_size = 2u; key_size <= 40; ++key_size)
    {
        std::vector<uint8_t> vec1;
        std::vector<uint8_t> vec2;
        vec1.reserve(key_size);
        vec2.reserve(key_size);

        auto it = vec.cbegin();//+key_size;
        for(auto ii = 0u; ii < key_size; ++ii)
        {
            vec1.push_back(*it++);
        }
        for(auto ii = 0u; ii < key_size; ++ii)
        {
            vec2.push_back(*it++);
        }

        auto hamming_distance = Hamming_distance(vec1, vec2);
        auto normalized_hamming_distance = static_cast<float>(hamming_distance) / key_size;
        if(normalized_hamming_distance < best_normalized_hamming_distance)
        {
            best_normalized_hamming_distance = normalized_hamming_distance;
            best_key_size = key_size;
        }

        cout << std::dec;   // TODO: should not be necessary.
        cout << "Size: " << key_size << ": distance: " << normalized_hamming_distance << endl;
    }

best_key_size = 29;

    std::vector<uint8_t> plaintext;
    plaintext.resize(vec.size());
    //plaintext.reserve(vec.size());    // TODO: are unallocated elements accessible?
    std::vector<uint8_t> key;
    //key.reserve(best_key_size);
    key.resize(best_key_size);

    for(auto ix = 0u; ix < best_key_size; ++ix)
    {
        std::vector<uint8_t> latch_vector;
        auto reserve_size = vec.size() / best_key_size + (ix < (vec.size() % best_key_size) ? 1 : 0);
        latch_vector.reserve(reserve_size);

        auto iter = vec.cbegin() + ix;
        for(auto ii = 0u; ii < reserve_size; ++ii)
        {
            latch_vector.push_back(*iter);
            if(ii < (reserve_size - 1))
            {
                iter += best_key_size;
            }
        }

        // Key should resolve to:
        // "Terminator X: Bring the noise" <- 29 characters.
//if(ix == 22) _asm int 3; // 'b' returned instead of 'e'.
        std::pair<unsigned int, uint8_t> key_score;
        //auto key_score = Find_single_byte_xor_key(latch_vector);
if(ix == 22)
key_score = Find_single_byte_xor_key(latch_vector);
else
key_score = Find_single_byte_xor_key(latch_vector);
        key[ix] = key_score.second;
        auto key_vector = Initialize_vector_with_array(&key_score.second, 1);
        auto latch_plain = Xor_sum_vectors(latch_vector, key_vector);

        auto plaintext_iter = plaintext.begin() + ix;
        for(auto latch_iter = latch_plain.cbegin(); latch_iter != latch_plain.cend(); ++latch_iter)
        {
            *plaintext_iter = *latch_iter;
            if(latch_iter + 1 != latch_plain.cend())
            {
                plaintext_iter += best_key_size;
            }
        }
    }

    cout << String_from_vector(plaintext) << endl;

    //if(false)
    //{
    //    cout << "Success." << endl;
    //}
    //else
    {
        cout << "FAILED." << endl;
    }

     cout << endl;
}

void Challenge7()
{
    using std::cout;
    using std::endl;

    cout << "Challenge 7: AES in ECB Mode." << endl;

    //if(false)
    //{
    //    cout << "Success." << endl;
    //}
    //else
    {
        cout << "FAILED." << endl;
    }

    cout << endl;
}

void Challenge8()
{
    using std::cout;
    using std::endl;

    cout << "Challenge 8: Detecting ECB." << endl;

    //if(false)
    //{
    //    cout << "Success." << endl;
    //}
    //else
    {
        cout << "FAILED." << endl;
    }

    cout << endl;
}

int main()
{
    // ERRORLEVEL zero is the success code.
    int error_level = 0;

    try
    {
        Challenge1();
        Challenge2();
        Challenge3();
        Challenge4();
        //Challenge5();
        Challenge6();
        //Challenge7();
        //Challenge8();
    }
    catch(const std::exception& ex)
    {
        std::fwprintf(stderr, L"\n%s\n", PortableRuntime::utf16_from_utf8(ex.what()).c_str());
        error_level = 1;
    }

    return error_level;
}


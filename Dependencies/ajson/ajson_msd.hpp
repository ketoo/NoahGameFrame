#pragma once

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <deque>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <ios>

#define STRINGFY_LIST(...) #__VA_ARGS__

namespace ajson
{
  namespace detail
  {
    struct string_ref
    {
      char const *  str = nullptr;
      size_t        len = 0;
      bool operator == (string_ref const& rhs) const
      {
        if (len == rhs.len)
        {
          return std::memcmp(str, rhs.str, len) == 0;
        }
        return false;
      }
    };

    typedef ::std::vector<string_ref> field_list;

    inline void add_field(char const * pre, char const * cur, field_list& fields)
    {
      size_t len = cur - pre;
      if (len > 0)
      {
        fields.emplace_back();
        auto& f = fields.back();
        f.str = pre;
        f.len = len;
      }
    }

    inline char const * char_table()
    {
      static char table[] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 16, 17, 17, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 17, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 16, 16, 16, 16, 16,
        16, 16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 10, 11, 12, 13, 14, 15 };
      return table;
    }

    inline bool is_ws(char c)
    {
      if (c > ' ')
        return false;
      return char_table()[c] == 17;
    }

    inline char const * skip_ws(char const * str)
    {
      while (is_ws(*str))++str;
      return str;
    }

    //input "v.abc,v.def,v.xyz"
    //output vector<string_ref> = ["abc","def","xyz"];
    inline field_list split_fields(char const * info)
    {
      field_list fields;
      char const * pre = info = skip_ws(info);
      while (*info != 0)
      {
        ++info;
        if (is_ws(*info))
        {
          add_field(pre, info, fields);
          info = skip_ws(info);
          ++info;
          pre = skip_ws(info);
        }
        else if (*info == ',')
        {
          add_field(pre, info, fields);
          ++info;
          pre = skip_ws(info);
        }
      }
      add_field(pre, info, fields);
      return fields;
    }

    template< class T >
    struct is_signed_intergral_like : std::integral_constant < bool,
      (std::is_integral<T>::value) &&
      std::is_signed<T>::value
    > {};

    template< class T >
    struct is_unsigned_intergral_like : std::integral_constant < bool,
      (std::is_integral<T>::value) &&
      std::is_unsigned<T>::value
    > {};

    template < template <typename...> class U, typename T >
    struct is_template_instant_of : std::false_type {};

    template < template <typename...> class U, typename... args >
    struct is_template_instant_of< U, U<args...> > : std::true_type{};

    template<typename T>
    struct is_stdstring : is_template_instant_of < std::basic_string, T >
    {};

    template< class T >
    struct is_sequence_container : std::integral_constant < bool,
      is_template_instant_of<std::deque, T>::value ||
      is_template_instant_of<std::list, T>::value ||
      is_template_instant_of<std::vector, T>::value ||
      is_template_instant_of<std::queue, T>::value
    > {};

    template< class T >
    struct is_associat_container : std::integral_constant < bool,
      is_template_instant_of<std::map, T>::value ||
      is_template_instant_of<std::unordered_map, T>::value
    > {};

    template< class T >
    struct is_emplace_back_able : std::integral_constant < bool,
      is_template_instant_of<std::deque, T>::value ||
      is_template_instant_of<std::list, T>::value ||
      is_template_instant_of<std::vector, T>::value
    > {};
  }

  struct token
  {
    detail::string_ref str;
    enum
    {
      t_string,
      t_int,
      t_uint,
      t_number,
      t_ctrl,
      t_end,
    } type;
    union
    {
      int64_t   i64;
      uint64_t  u64;
      double    d64;
    } value;
    bool neg = false;
  };

#ifdef _MSC_VER
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

  class exception : public std::exception
  {
    std::string msg_;
  public:
    exception(std::string const& msg)
      :msg_(msg)
    {}
    char const * what() const NOEXCEPT{ return msg_.c_str(); }
  };

  class reader
  {
    token   cur_tok_;
    size_t  cur_col_ = 0;
    size_t  cur_line_ = 0;
    size_t  len_ = 0;
    size_t  cur_offset_ = 0;
    bool    end_mark_ = false;
    char  * ptr_ = nullptr;
    double decimal = 0.1;
    int    exp = 0;
    inline void decimal_reset(){ decimal = 0.1; }

    inline char read() const
    {
      if (end_mark_)
        return 0;
      return ptr_[cur_offset_];
    }

    inline void take()
    {
      if (end_mark_ == false)
      {
        ++cur_offset_;
        char v = ptr_[cur_offset_];
        if (v != '\r')
          ++cur_col_;
        if (len_ > 0 && cur_offset_ >= len_)
        {
          end_mark_ = true;
        }
        else if (v == 0)
        {
          end_mark_ = true;
        }
        if (v == '\n')
        {
          cur_col_ = 0;
          ++cur_line_;
        }
      }
    }

    char skip()
    {
      auto c = read();
      while (c == ' ' || c == '\t' || c == '\r' || c == '\n')
      {
        take();
        c = read();
      }
      return c;
    }

    inline void fill_escape_char(size_t count, char c)
    {
      if (count == 0)
        return;
      ptr_[cur_offset_ - count] = c;
    }

    inline char char_to_hex(char v)
    {
      if (v <= 'f')
      {
        v = detail::char_table()[v];
      }
      else
      {
        v = 16;
      }
      if (v > 15)
        error("utf8 code error!");
      return v;
    }

    inline uint64_t read_utf()
    {
      char v = char_to_hex(read());
      uint64_t utf = v;
      utf <<= 4;
      take();
      v = char_to_hex(read());
      utf += v;
      utf <<= 4;
      take();
      v = char_to_hex(read());
      utf += v;
      utf <<= 4;
      take();
      v = char_to_hex(read());
      utf += v;
      take();
      return utf;
    }

    inline void esacpe_utf8(size_t& esc_count)
    {
      uint64_t utf1 = read_utf();
      esc_count += 6;

      if (utf1 < 0x80)
      {
        fill_escape_char(esc_count, (char)utf1);
        --esc_count;
      }
      else if (utf1 < 0x800)
      {
        fill_escape_char(esc_count, (char)(0xC0 | ((utf1 >> 6) & 0xFF)));
        fill_escape_char(esc_count - 1, (char)(0x80 | ((utf1 & 0x3F))));
        esc_count -= 2;
      }
      else if (utf1 < 0x80000)
      {
        fill_escape_char(esc_count, (char)(0xE0 | ((utf1 >> 12) & 0xFF)));
        fill_escape_char(esc_count - 1, (char)(0x80 | ((utf1 >> 6) & 0x3F)));
        fill_escape_char(esc_count - 2, (char)(0x80 | ((utf1 & 0x3F))));
        esc_count -= 3;
      }
      else
      {
        if (utf1 < 0x110000)
        {
          error("utf8 code error!");
        }
        fill_escape_char(esc_count, (char)(0xF0 | ((utf1 >> 18) & 0xFF)));
        fill_escape_char(esc_count - 1, (char)(0x80 | ((utf1 >> 12) & 0x3F)));
        fill_escape_char(esc_count - 2, (char)(0x80 | ((utf1 >> 6) & 0x3F)));
        fill_escape_char(esc_count - 3, (char)(0x80 | ((utf1 & 0x3F))));
        esc_count -= 4;
      }
    }

    void parser_quote_string()
    {
      take();
      cur_tok_.str.str = ptr_ + cur_offset_;
      auto c = read();
      size_t esc_count = 0;
      do
      {
        switch (c)
        {
        case 0:
        case '\n':
        {
          error("not a valid quote string!");
          break;
        }
        case '\\':
        {
          take();
          c = read();
          switch (c)
          {
          case '\\':
          {
            c = '\\';
            break;
          }
          case '/':
          {
            c = '/';
            break;
          }
          case 'b':
          {
            c = '\b';
            break;
          }
          case 'f':
          {
            c = '\f';
            break;
          }
          case 'n':
          {
            c = '\n';
            break;
          }
          case 'r':
          {
            c = '\r';
            break;
          }
          case 't':
          {
            c = '\t';
            break;
          }
          case '"':
          {
            break;
          }
          case 'u':
          {
            take();
            esacpe_utf8(esc_count);
            c = read();
            continue;
          }
          default:
          {
            error("unknown escape char!");
          }
          }
          ++esc_count;
          break;
        }
        case '"':
        {
          cur_tok_.str.len = ptr_ + cur_offset_ - esc_count - cur_tok_.str.str;
          take();
          return;
        }
        }
        fill_escape_char(esc_count, c);
        take();
        c = read();
      } while (true);
    }

    void parser_string()
    {
      cur_tok_.str.str = ptr_ + cur_offset_;
      take();
      auto c = read();
      size_t esc_count = 0;
      do
      {
        switch (c)
        {
        case 0:
        {
          error("not a valid string!");
          break;
        }
        case '\\':
        {
          take();
          c = read();
          switch (c)
          {
          case 'b':
          {
            c = '\b';
            break;
          }
          case 'f':
          {
            c = '\f';
            break;
          }
          case 'n':
          {
            c = '\n';
            break;
          }
          case 'r':
          {
            c = '\r';
            break;
          }
          case 't':
          {
            c = '\t';
            break;
          }
          case 'u':
          {
            take();
            esacpe_utf8(esc_count);
            continue;
          }
          default:
          {
            error("unknown escape char!");
          }
          }
          ++esc_count;
          break;
        }
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case ',':
        case '[':
        case ']':
        case ':':
        case '{':
        case '}':
        {
          cur_tok_.str.len = ptr_ + cur_offset_ - esc_count - cur_tok_.str.str;
          return;
        }
        }
        fill_escape_char(esc_count, c);
        take();
        c = read();
      } while (true);
    }

    void parser_exp_pos()
    {
      take();
      auto c = read();
      do
      {
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          exp *= 10;
          exp += (c - '0');
          break;
        }
        default:
        {
          for (int i = 0; i < exp; ++i)
          {
            cur_tok_.value.d64 *= 10.0;
          }
          exp = 0;
          cur_tok_.str.len = ptr_ + cur_offset_ - cur_tok_.str.str;
          return;
        }
        }
        take();
        c = read();
      } while (1);
    }

    void parser_exp_neg()
    {
      take();
      auto c = read();
      do
      {
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          exp *= 10;
          exp += (c - '0');
          break;
        }
        default:
        {
          for (int i = 0; i < exp; ++i)
          {
            cur_tok_.value.d64 *= 0.1;
          }
          exp = 0;
          cur_tok_.str.len = ptr_ + cur_offset_ - cur_tok_.str.str;
          return;
        }
        }
        take();
        c = read();
      } while (1);
    }

    void parser_exp()
    {
      take();
      auto c = read();
      do
      {
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          exp *= 10;
          exp += (c - '0');
          parser_exp_pos();
          return;
        }
        case '-':
        {
          parser_exp_neg();
          return;
        }
        default:
        {
          exp = 0;
          cur_tok_.str.len = ptr_ + cur_offset_ - cur_tok_.str.str;
          return;
        }
        }
      } while (1);
    }

    void parser_number()
    {
      cur_tok_.str.str = ptr_ + cur_offset_;
      take();
      auto c = read();
      do
      {
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          if (cur_tok_.type == token::t_int)
          {
            cur_tok_.value.i64 *= 10;
            cur_tok_.value.i64 += c - '0';
          }
          else if (cur_tok_.type == token::t_uint)
          {
            cur_tok_.value.u64 *= 10;
            cur_tok_.value.u64 += c - '0';
          }
          else if (cur_tok_.type == token::t_number)
          {
            cur_tok_.value.d64 += decimal * (c - '0');
            decimal *= 0.1;
          }
          break;
        }
        case '.':
        {
          if (cur_tok_.type == token::t_int)
          {
            cur_tok_.type = token::t_number;
            cur_tok_.value.d64 = (double)cur_tok_.value.i64;
            decimal_reset();
          }
          else if (cur_tok_.type == token::t_uint)
          {
            cur_tok_.type = token::t_number;
            cur_tok_.value.d64 = (double)cur_tok_.value.u64;
            decimal_reset();
          }
          else if (cur_tok_.type == token::t_number)
          {
            error("not a valid number!");
          }
          break;
        }
        case 'e':
        case 'E':
        {
          if (cur_tok_.type == token::t_int)
          {
            cur_tok_.type = token::t_number;
            cur_tok_.value.d64 = (double)cur_tok_.value.i64;
          }
          else if (cur_tok_.type == token::t_uint)
          {
            cur_tok_.type = token::t_number;
            cur_tok_.value.d64 = (double)cur_tok_.value.u64;
          }
          parser_exp();
        }
        default:
        {
          cur_tok_.str.len = ptr_ + cur_offset_ - cur_tok_.str.str;
          return;
        }
        }
        take();
        c = read();
      } while (1);
    }
  public:
    reader(char * ptr = nullptr, size_t len = -1)
      :ptr_(ptr), len_(len)
    {
      if (ptr == nullptr)
      {
        end_mark_ = true;
      }
      else if (len == 0)
      {
        end_mark_ = true;
      }
      else if (ptr[0] == 0)
      {
        end_mark_ = true;
      }
      next();
    }

    static inline char* itoa_native(size_t val , char * buffer , size_t len)
    {
      buffer[len] = 0;
      size_t pos = len - 1;
      if (val == 0)
      {
        buffer[pos--] = '0';
      }
      while (val)
      {
        buffer[pos--] = (char)((val % 10) + '0');
        val = val / 10;
      }
      ++pos;
      return &buffer[0] + pos;
    }

    inline void error(const char * message) const
    {
      char buffer[20];
      std::string msg = "error at line :";
      msg += itoa_native(cur_line_,buffer,19);
      msg += " col :";
      msg += itoa_native(cur_col_, buffer, 19);
      msg += " msg:";
      msg += message;
      throw exception(msg);
    }

    inline token const&  peek() const
    {
      return cur_tok_;
    }

    void next()
    {
      auto c = skip();
      bool do_next = false;
      cur_tok_.neg = false;
      switch (c)
      {
      case 0:
        cur_tok_.type = token::t_end;
        cur_tok_.str.str = ptr_ + cur_offset_;
        cur_tok_.str.len = 1;
        break;
      case '{':
      case '}':
      case '[':
      case ']':
      case ':':
      case ',':
      {
        cur_tok_.type = token::t_ctrl;
        cur_tok_.str.str = ptr_ + cur_offset_;
        cur_tok_.str.len = 1;
        take();
        break;
      }
      case '/':
      {
        take();
        c = read();
        if (c == '/')
        {
          take();
          c = read();
          while (c != '\n' && c != 0)
          {
            take();
            c = read();
          }
          do_next = true;
          break;
        }
        else if (c == '*')
        {
          take();
          c = read();
          do
          {
            while (c != '*')
            {
              if (c == 0)
              {
                return;
              }
              take();
              c = read();
            }
            take();
            c = read();
            if (c == '/')
            {
              take();
              do_next = true;
              break;
            }
          } while (true);
        }
        //error parser comment
        error("not a comment!");
      }
      case '"':
      {
        cur_tok_.type = token::t_string;
        parser_quote_string();
        break;
      }
      default:
      {
        if (c >= '0' && c <= '9')
        {
          cur_tok_.type = token::t_uint;
          cur_tok_.value.u64 = c - '0';
          parser_number();
        }
        else if (c == '-')
        {
          cur_tok_.type = token::t_int;
          cur_tok_.value.i64 = 0;
          cur_tok_.neg = true;
          parser_number();
        }
        else
        {
          cur_tok_.type = token::t_string;
          parser_string();
        }
      }
      }
      if (do_next == false)
        return;
      next();
    }

    inline bool expect(char c)
    {
      return cur_tok_.str.str[0] == c;
    }
  };

  template <typename alloc_ty>
  struct ajson_string_stream
  {
  private:
    alloc_ty alloc;
  public:
    enum { good, read_overflow };

    char * m_header_ptr;
    char * m_read_ptr;
    char * m_write_ptr;
    char * m_tail_ptr;
    int							m_status;
    std::size_t			m_length;

    enum{ INIT_BUFF_SIZE = 1024 };
    ajson_string_stream() :m_length(INIT_BUFF_SIZE), m_status(good)
    {
      this->m_header_ptr = this->alloc.allocate(INIT_BUFF_SIZE);
      this->m_read_ptr = this->m_header_ptr;
      this->m_write_ptr = this->m_header_ptr;
      this->m_tail_ptr = this->m_header_ptr + m_length;
    }

    ~ajson_string_stream()
    {
      this->alloc.deallocate(m_header_ptr, this->m_length);
    }

    inline std::size_t read(char * buffer, std::size_t len)
    {
      if (this->m_read_ptr + len > this->m_tail_ptr)
      {
        m_status = read_overflow;
        return 0;
      }
      std::memcpy(buffer, this->m_read_ptr, len);
      this->m_read_ptr += len;
      return len;
    }

    inline std::size_t growpup(std::size_t want_size)
    {
      std::size_t new_size = ((want_size + INIT_BUFF_SIZE - 1) / INIT_BUFF_SIZE)*INIT_BUFF_SIZE;
      std::size_t write_pos = this->m_write_ptr - this->m_header_ptr;
      std::size_t read_pos = this->m_read_ptr - this->m_header_ptr;
      char * temp = this->m_header_ptr;
      this->m_header_ptr = this->alloc.allocate(new_size);
      std::memcpy(this->m_header_ptr, temp, this->m_length);
      this->alloc.deallocate(temp, this->m_length);
      this->m_length = new_size;
      this->m_write_ptr = this->m_header_ptr + write_pos;
      this->m_read_ptr = this->m_header_ptr + read_pos;
      this->m_tail_ptr = this->m_header_ptr + m_length;
      return new_size;
    }

    inline std::size_t write(const char * buffer, std::size_t len)
    {
      std::size_t writed_len = this->m_write_ptr + len - this->m_header_ptr;
      if (writed_len > this->m_length)
      {
        this->growpup(writed_len);
      }
      std::memcpy((void*)this->m_write_ptr, buffer, len);
      this->m_write_ptr += len;
      return len;
    }

    inline void putc(char c)
    {
      std::size_t writed_len = this->m_write_ptr + 1 - this->m_header_ptr;
      if (writed_len > this->m_length)
      {
        this->growpup(writed_len);
      }
      *this->m_write_ptr = c;
      ++this->m_write_ptr;
    }


    inline bool bad()const{ return m_status != good; }

    inline ajson_string_stream& seekp(int offset, int seek_dir)
    {
      switch (seek_dir)
      {
      case std::ios::beg:
      {
        if (offset < 0)
        {
          offset = 0;
        }
        this->m_write_ptr = this->m_header_ptr + offset;
        break;
      }
      case std::ios::cur:
      {
        if (offset < 0)
        {
          offset = offset + int(this->m_write_ptr - this->m_header_ptr);
          if (offset < 0)
          {
            offset = 0;
          }
          this->m_write_ptr = this->m_header_ptr + offset;
        }
        else
        {
          if (this->m_write_ptr + offset > this->m_tail_ptr)
          {
            this->m_write_ptr = this->m_tail_ptr;
          }
        }

        break;
      }
      case std::ios::end:
      {
        if (offset < 0)
        {
          offset = offset + int(this->m_write_ptr - this->m_header_ptr);
          if (offset < 0)
          {
            offset = 0;
          }
          this->m_write_ptr = this->m_header_ptr + offset;
        }
        break;
      }
      }
      return *this;
    }

    inline void clear()
    {
      this->m_read_ptr = this->m_header_ptr;
      this->m_write_ptr = this->m_header_ptr;
    }

    inline const char * data() const
    {
      return this->m_header_ptr;
    }

    std::basic_string<char, std::char_traits<char>, alloc_ty> str()
    {
      std::basic_string<char, std::char_traits<char>, alloc_ty> s(this->m_header_ptr, this->write_length());
      return s;
    }

    inline ::std::size_t read_length() const
    {
      return this->m_read_ptr - this->m_header_ptr;
    }

    inline ::std::size_t write_length() const
    {
      return this->m_write_ptr - this->m_header_ptr;
    }
  };

  typedef ajson_string_stream<std::allocator<char> > string_stream;

  struct ajson_file_stream
  {
  public:

    enum { good, read_overflow, file_error };

    FILE * m_f;
    int		 m_status;
    enum{ INIT_AMSG_BUFF_SIZE = 1024 };
    ajson_file_stream(const char * filename) :m_f(NULL), m_status(good)
    {
      this->m_f = std::fopen(filename, "w");
      if (NULL == this->m_f)
      {
        this->m_status = file_error;
      }
    }

    ~ajson_file_stream()
    {
      if (this->m_f)
      {
        fclose(m_f);
      }
    }

    inline std::size_t read(char * buffer, std::size_t len)
    {
      std::size_t rlen = std::fread(buffer, len, 1, this->m_f);
      return rlen;
    }

    inline std::size_t write(const char * buffer, std::size_t len)
    {
      std::size_t wlen = std::fwrite(buffer, len, 1, this->m_f);
      return wlen;
    }

    inline void putc(char c)
    {
      std::fputc(c, this->m_f);
    }

    inline bool bad(){ return m_status != good; }

    inline int seekp(int offset, int seek_dir)
    {
      return std::fseek(this->m_f, offset, seek_dir);
    }

    inline void clear()
    {
      std::fseek(this->m_f, 0, SEEK_SET);
    }
  };

  template<typename stream_ty>
  struct lite_write
  {
    stream_ty& s_;
    lite_write(stream_ty& s) :s_(s){}

    inline void write_bool(bool v)
    {
      if (v)
        s_.putc('1');
      else
        s_.putc('0');
    }

    inline void write_liter(char const * str, size_t len)
    {
      s_.write(str, len);
    }

    inline void write_str(char const * str, size_t len)
    {
      static char const * hex_table = "0123456789ABCDEF";
      static char const escape[256] = {
#define Z16 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        //0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
        'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00
        'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10
        0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20
        Z16, Z16,																		// 30~4F
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\\', 0, 0, 0, // 50
        Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16								// 60~FF
#undef Z16
      };

      s_.putc('"');
      char const * ptr = str;
      char const * end = ptr + len;
      while (ptr < end)
      {
        const char c = *ptr;
        if(c==0)
          break;
        ++ptr;
        if (escape[(unsigned char)c])
        {
          char buff[6] = { '\\', '0' };
          size_t len = 2;
          buff[1] = escape[(unsigned char)c];
          if (buff[1] == 'u')
          {
            if(ptr < end)
            {
              buff[2] = (hex_table[((unsigned char)c) >> 4]);
              buff[3] = (hex_table[((unsigned char)c) & 0xF]);
              const char c1 = *ptr;
              ++ptr;
              buff[4] = (hex_table[((unsigned char)c1) >> 4]);
              buff[5] = (hex_table[((unsigned char)c1) & 0xF]);
            }
            else
            {
              buff[2] = '0';
              buff[3] = '0';
              buff[4] = (hex_table[((unsigned char)c) >> 4]);
              buff[5] = (hex_table[((unsigned char)c) & 0xF]);
            }
            len = 6;
          }
          s_.write(buff, len);
        }
        else
        {
          s_.putc(c);
        }
      }
      s_.putc('"');
    }

    inline void putc(char c)
    {
      s_.putc(c);
    }
  };

  template<typename ty, class enable = void>
  struct json_impl;

  inline bool is_true(token const& tok)
  {
    char const * ptr = tok.str.str;
    if (tok.str.len == 4)
    {
      return (ptr[0] == 't' || ptr[0] == 'T') &&
        (ptr[1] == 'r' || ptr[1] == 'R') &&
        (ptr[2] == 'u' || ptr[2] == 'U') &&
        (ptr[3] == 'e' || ptr[3] == 'E');
    }
    return false;
  }

  template<>
  struct json_impl < bool, void >
  {
    static inline void read(reader& rd, bool& val)
    {
      auto& tok = rd.peek();
      switch (tok.type)
      {
      case token::t_string:
      {
        val = is_true(tok);
        break;
      }
      case token::t_int:
      {
        val = (tok.value.i64 != 0);
        break;
      }
      case token::t_uint:
      {
        val = (tok.value.u64 != 0);
        break;
      }
      case token::t_number:
      {
        val = (tok.value.d64 != 0.0);
        break;
      }
      default:
      {
        rd.error("not a valid bool.");
      }
      }
      rd.next();
    }

    template<typename write_ty>
    static inline void write(write_ty& wt, bool const& val)
    {
      wt.write_bool(val);
    }

    template<typename write_ty>
    static inline void write_key(write_ty& wt, bool const& val)
    {
      wt.putc('"');
      wt.write_bool(val);
      wt.putc('"');
    }
  };

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <detail::is_signed_intergral_like<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      auto& tok = rd.peek();
      switch (tok.type)
      {
      case token::t_string:
      {
        if (is_true(tok))
        {
          val = 1;
        }
        else
        {
          int64_t temp = std::strtoll(tok.str.str, nullptr, 10);
          val = static_cast<ty>(temp);
        }
        break;
      }
      case token::t_int:
      {
        val = static_cast<ty>(tok.value.i64);
        if (tok.neg)
          val = -val;
        break;
      }
      case token::t_uint:
      {
        val = static_cast<ty>(tok.value.u64);
        break;
      }
      case token::t_number:
      {
        val = static_cast<ty>(tok.value.d64);
        if (tok.neg)
          val = -val;
        break;
      }
      default:
      {
        rd.error("not a valid signed integral like number.");
      }
      }
      rd.next();
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      int64_t temp = (int64_t)val;
      char buffer[64];
      buffer[63] = 0;
      size_t len = 64;
      size_t pos = 62;
      bool Sig = false;
      if (temp < 0)
      {
        Sig = true;
        temp *= -1;
      }
      if (temp == 0)
      {
        buffer[pos--] = '0';
      }
      while (temp)
      {
        buffer[pos--] = (char)((temp % 10) + '0');
        temp = temp / 10;
      }
      if (Sig)
      {
        buffer[pos--] = '-';
      }
      ++pos;
      wt.write_liter(buffer + pos, len - pos - 1);
    }

    template<typename write_ty>
    static inline void write_key(write_ty& wt, ty const& val)
    {
      wt.putc('"');
      write<write_ty>(wt, val);
      wt.putc('"');
    }
  };

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <detail::is_unsigned_intergral_like<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      auto& tok = rd.peek();
      switch (tok.type)
      {
      case token::t_string:
      {
        if (is_true(tok))
        {
          val = 1;
        }
        else
        {
          uint64_t temp = std::strtoull(tok.str.str, nullptr, 10);
          val = static_cast<ty>(temp);
        }
        break;
      }
      case token::t_int:
      {
        if (tok.neg)
        {
          rd.error("assign a negative signed integral to unsigned integral number.");
        }
        val = static_cast<ty>(tok.value.i64);
        break;
      }
      case token::t_uint:
      {
        val = static_cast<ty>(tok.value.u64);
        break;
      }
      case token::t_number:
      {
        if (tok.neg)
        {
          rd.error("assign a negative float point to unsigned integral number.");
        }
        val = static_cast<ty>(tok.value.d64);
        break;
      }
      default:
      {
        rd.error("not a valid unsigned integral like number.");
      }
      }
      rd.next();
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      uint64_t temp = (uint64_t)val;
      char buffer[64];
      buffer[63] = 0;
      size_t len = 64;
      size_t pos = 62;
      if (temp == 0)
      {
        buffer[pos--] = '0';
      }
      while (temp)
      {
        buffer[pos--] = (char)((temp % 10) + '0');
        temp = temp / 10;
      }
      ++pos;
      wt.write_liter(buffer + pos, len - pos - 1);
    }

    template<typename write_ty>
    static inline void write_key(write_ty& wt, ty const& val)
    {
      wt.putc('"');
      write<write_ty>(wt, val);
      wt.putc('"');
    }
  };

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <std::is_enum<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      typedef typename std::underlying_type<ty>::type raw_type;
      json_impl<raw_type>::read(rd, (raw_type&)val);
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      typedef typename std::underlying_type<ty>::type raw_type;
      json_impl<raw_type>::write(wt, val);
    }

    template<typename write_ty>
    static inline void write_key(write_ty& wt, ty const& val)
    {
      typedef typename std::underlying_type<ty>::type raw_type;
      json_impl<raw_type>::write_key(wt, val);
    }
  };

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <std::is_floating_point<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      auto& tok = rd.peek();
      switch (tok.type)
      {
      case token::t_string:
      {
        double temp = std::strtold(tok.str.str, nullptr);
        val = static_cast<ty>(temp);
        break;
      }
      case token::t_int:
      {
        val = static_cast<ty>(tok.value.i64);
        if (tok.neg)
          val = -val;
        break;
      }
      case token::t_uint:
      {
        val = static_cast<ty>(tok.value.u64);
        break;
      }
      case token::t_number:
      {
        val = static_cast<ty>(tok.value.d64);
        if (tok.neg)
          val = -val;
        break;
      }
      default:
      {
        rd.error("not a valid float point number.");
      }
      }
      rd.next();
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      char buffer[64] = { 0 };
#ifdef _MSC_VER
      _gcvt_s(buffer, 63 , val, 8);
#else
      gcvt(val, 62, buffer);
#endif // MSVC
      size_t len = std::strlen(buffer);
      if (buffer[len - 1] == '.')
      {
        buffer[len - 1] = '\0';
        -- len;
      }
      wt.write_liter(buffer, len);
    }

    template<typename write_ty>
    static inline void write_key(write_ty& wt, ty const& val)
    {
      wt.putc('"');
      write<write_ty>(wt, val);
      wt.putc('"');
    }
  };

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <detail::is_stdstring<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      auto& tok = rd.peek();
      if (tok.type == token::t_string)
      {
        val.assign(tok.str.str, tok.str.len);
      }
      else
      {
        rd.error("not a valid string.");
      }
      rd.next();
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      wt.write_str(val.data(), val.length());
    }

    template<typename write_ty>
    static inline void write_key(write_ty& wt, ty const& val)
    {
      write<write_ty>(wt, val);
    }
  };

  inline void char_array_read(reader& rd, char * val, int N)
  {
    auto& tok = rd.peek();
    if (tok.type == token::t_string)
    {
      size_t len = tok.str.len;
      if (len > N)
        len = N;
      std::memcpy(val, tok.str.str, len);
      if (len < N)
        val[len] = 0;
    }
    else
    {
      rd.error("not a valid string.");
    }
    rd.next();
  }

  template<typename write_ty>
  inline void char_array_write(write_ty& wt, const char * val, int N)
  {
    wt.write_str(val, N);
  }

  template<size_t N>
  struct json_impl <char[N]>
  {
    static inline void read(reader& rd, char * val)
    {
      char_array_read(rd, val, N);
    }

    template<typename write_ty>
    static inline void write(write_ty& wt, const char * val)
    {
      char_array_write(wt, val, N);
    }
  };

  template<size_t N>
  struct json_impl <const char[N] >
  {
    static inline void read(reader& rd, char * val)
    {
      char_array_read(rd, val, N);
    }

    template<typename write_ty>
    static inline void write(write_ty& wt, const char * val)
    {
      char_array_write(wt, val, N);
    }
  };

  template<typename T>
  inline void array_read(reader& rd, T * val, int N)
  {
    if (rd.expect('[') == false)
    {
      rd.error("array must start with [.");
    }
    rd.next();
    auto tok = &rd.peek();
    size_t count = 0;
    while (tok->str.str[0] != ']')
    {
      if (count < N)
      {
        json_impl<T>::read(rd, val[count]);
      }
      else
      {
        rd.error("array overflow.");
      }
      ++count;
      tok = &rd.peek();
      if (tok->str.str[0] == ',')
      {
        rd.next();
        tok = &rd.peek();
        continue;
      }
      else if (tok->str.str[0] == ']')
      {
        break;
      }
      else
      {
        rd.error("no valid array!");
      }
    }
    rd.next();
    return;
  }

  template<typename T, typename write_ty>
  inline void array_write(write_ty& wt, const T * val, int N)
  {
    wt.putc('[');
    int last = N - 1;
    for (int i = 0; i < N; ++i)
    {
      json_impl<T>::write(wt, val[i]);
      if (i < last)
        wt.putc(',');
    }
    wt.putc(']');
  }

  template<typename T, size_t N>
  struct json_impl <T[N]>
  {
    static inline void read(reader& rd, T * val)
    {
      array_read(rd, val, N);
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, const T * val)
    {
      array_write(wt, val, N);
    }
  };

  template<typename T, size_t N>
  struct json_impl < const T[N]>
  {
    static inline void read(reader& rd, T * val)
    {
      array_read(rd, val, N);
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, const T * val)
    {
      array_write(wt, val, N);
    }
  };

  template<typename ty>
  inline typename std::enable_if <detail::is_emplace_back_able<ty>::value, void>::type
    emplace_back(ty& val)
  {
    val.emplace_back();
  }

  template<typename ty>
  inline typename std::enable_if <detail::is_template_instant_of<std::queue, ty>::value, void>::type
    emplace_back(ty& val)
  {
    val.emplace();
  }

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <detail::is_sequence_container<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      if (rd.expect('[') == false)
      {
        rd.error("array must start with [.");
      }
      rd.next();
      auto tok = &rd.peek();
      while (tok->str.str[0] != ']')
      {
        ::ajson::emplace_back(val);
        json_impl<typename ty::value_type>::read(rd, val.back());
        tok = &rd.peek();
        if (tok->str.str[0] == ',')
        {
          rd.next();
          tok = &rd.peek();
          continue;
        }
        else if (tok->str.str[0] == ']')
        {
          break;
        }
        else
        {
          rd.error("no valid array!");
        }
      }
      rd.next();
      return;
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      wt.putc('[');
      auto sz = val.size();
      for (auto& i : val)
      {
        json_impl<typename ty::value_type>::write(wt, i);
        if (sz-- > 1)
          wt.putc(',');
      }
      wt.putc(']');
    }
  };

  template<typename ty>
  struct json_impl < ty,
    typename std::enable_if <detail::is_associat_container<ty>::value>::type >
  {
    static inline void read(reader& rd, ty& val)
    {
      if (rd.expect('{') == false)
      {
        rd.error("object must start with {!");
      }
      rd.next();
      auto tok = &rd.peek();
      while (tok->str.str[0] != '}')
      {
        typename ty::key_type key;
        typename ty::mapped_type value;
        json_impl<typename ty::key_type>::read(rd, key);
        if (rd.expect(':') == false)
        {
          rd.error("invalid object!");
        }
        rd.next();
        json_impl<typename ty::mapped_type>::read(rd, value);
        val[key] = value;
        tok = &rd.peek();
        if (tok->str.str[0] == ',')
        {
          rd.next();
          tok = &rd.peek();
          continue;
        }
        else if (tok->str.str[0] == '}')
        {
          break;
        }
        else
        {
          rd.error("no valid object!");
        }
      }
      rd.next();
      return;
    }
    template<typename write_ty>
    static inline void write(write_ty& wt, ty const& val)
    {
      wt.putc('{');
      auto sz = val.size();
      for (auto& i : val)
      {
        json_impl<typename ty::key_type>::write_key(wt, i.first);
        wt.putc(':');
        json_impl<typename ty::mapped_type>::write(wt, i.second);
        if (sz-- > 1)
          wt.putc(',');
      }
      wt.putc('}');
    }
  };

  inline void skip_array(reader& rd);

  inline void skip_object(reader& rd);

  inline void skip(reader& rd)
  {
    auto& tok = rd.peek();
    switch (tok.type)
    {
    case token::t_string:
    case token::t_int:
    case token::t_uint:
    case token::t_number:
    {
      rd.next();
      return;
    }
    case token::t_ctrl:
    {
      if (tok.str.str[0] == '[')
      {
        rd.next();
        skip_array(rd);
        return;
      }
      else if (tok.str.str[0] == '{')
      {
        rd.next();
        skip_object(rd);
        return;
      }
    }
    case token::t_end:
    {
      return;
    }
    }
    rd.error("invalid json document!");
  }

  inline void skip_array(reader& rd)
  {
    auto tok = &rd.peek();
    while (tok->str.str[0] != ']')
    {
      skip(rd);
      tok = &rd.peek();
      if (tok->str.str[0] == ',')
      {
        rd.next();
        tok = &rd.peek();
        continue;
      }
    }
    rd.next();
  }

  inline void skip_key(reader& rd)
  {
    auto& tok = rd.peek();
    switch (tok.type)
    {
    case token::t_string:
    case token::t_int:
    case token::t_uint:
    case token::t_number:
    {
      rd.next();
      return;
    }
    default:
      break;
    }
    rd.error("invalid json document!");
  }

  inline void skip_object(reader& rd)
  {
    auto tok = &rd.peek();
    while (tok->str.str[0] != '}')
    {
      skip_key(rd);
      tok = &rd.peek();
      if (tok->str.str[0] == ':')
      {
        rd.next();
        skip(rd);
        //rd.next();
        tok = &rd.peek();
      }
      else
      {
        rd.error("invalid json document!");
      }
      if (tok->str.str[0] == ',')
      {
        rd.next();
        tok = &rd.peek();
        continue;
      }
    }
    rd.next();
  }

  template<typename head, typename... args>
  inline int read_members(reader& rd, detail::string_ref const * member_ptr,
    detail::string_ref const& member, size_t pos, head& h, args& ... args_);

  inline int read_members(reader&, detail::string_ref const*,
    detail::string_ref const&, size_t);

  template<typename head, typename... args>
  struct read_members_impl
  {
    static inline int read(reader& rd, detail::string_ref const * member_ptr,
      detail::string_ref const& member, size_t pos, head& val, args& ... args_)
    {
      if (member_ptr[pos] == member)
      {
        json_impl<head>::read(rd, val);
        return 1;
      }
      if (sizeof...(args))
        return read_members(rd, member_ptr, member, pos + 1, args_...);
      return 0;
    }
  };

  template<typename head, typename... args>
  inline int read_members(reader& rd, detail::string_ref const * member_ptr,
    detail::string_ref const& member, size_t pos, head& h, args& ... args_)
  {
    return read_members_impl<head, args...>::read(rd, member_ptr,
      member, pos, h, args_...);
  }

  inline int read_members(reader&, detail::string_ref const*,
    detail::string_ref const&, size_t)
  {
    return 0;
  }

  template<typename ty>
  inline void load_from_buff(ty& val, char * buff, size_t len = -1)
  {
    typedef typename std::remove_cv<ty>::type rty;
    reader rd(buff, len);
    json_impl<rty>::read(rd, val);
  }

  template<typename ty>
  inline void load_from_file(ty& val, const char * filename)
  {
    struct file_guard
    {
      std::FILE * f_;
      file_guard(std::FILE * f):f_(f){}
      ~file_guard()
      {
        if (f_)
          std::fclose(f_);
      }
    };
    std::FILE * f = std::fopen(filename, "rb");
    if (nullptr == f)
    {
      std::string errmsg = "can't open file:";
      throw std::logic_error( errmsg + filename);
    }
    file_guard fg(f);
    std::fseek(f, 0, SEEK_END);
    auto sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    struct buffer_guard
    {
      char * buff_ = nullptr;
      buffer_guard(char * buff) :buff_(buff){}
      ~buffer_guard()
      {
        if (buff_)
          delete [] buff_;
      }
    };

    char * buffer = new char[sz+1];
    buffer_guard bg(buffer);
    sz = std::fread(buffer, 1, sz, f);
    buffer[sz] = 0;
    if (sz >= 3)
    {
      if (buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF)
      {
        buffer += 3;
        sz -= 3;
      }
    }
    reader rd(buffer, sz);
    typedef typename std::remove_cv<ty>::type rty;
    json_impl<rty>::read(rd, val);
  }

  template<typename write_ty, typename head, typename... args>
  inline void write_members(write_ty& wt, detail::string_ref const * member_ptr,
    size_t pos, head& h, args& ... args_);

  template<typename write_ty>
  inline void write_members(write_ty&, detail::string_ref *, size_t);

  template<typename write_ty, typename head, typename... args>
  struct write_members_impl
  {
    static inline void write(write_ty& wt, detail::string_ref const * member_ptr,
      size_t pos, head const& val, args const& ... args_)
    {
      wt.write_str(member_ptr[pos].str, member_ptr[pos].len);
      wt.putc(':');
      json_impl<head>::write(wt, val);
      if (sizeof...(args))
      {
        wt.putc(',');
        write_members(wt, member_ptr, pos + 1, args_...);
      }
    }
  };

  template<typename write_ty, typename head, typename... args>
  inline void write_members(write_ty& wt, detail::string_ref const* member_ptr,
    size_t pos, head const& h, args& ... args_)
  {
    return write_members_impl<write_ty, head, args...>::write(wt, member_ptr,
      pos, h, args_...);
  }

  template<typename write_ty>
  inline void write_members(write_ty&, detail::string_ref const*, size_t)
  {
    return;
  }

  template<typename ty, typename stream_ty, class write_tp = lite_write<stream_ty> >
  inline void save_to(stream_ty& ss, ty& val)
  {
    typedef typename std::remove_cv<ty>::type rty;
    write_tp wt(ss);
    json_impl<rty>::write(wt, val);
  }

  template<typename ty, typename stream_ty = ajson_file_stream, class write_tp = lite_write<stream_ty> >
  inline void save_to_file(ty& val, char const * filename)
  {
    stream_ty fs(filename);
    save_to < ty, stream_ty, write_tp>(fs, val);
  }
}

#define AJSON(TYPE,...) \
namespace ajson\
{\
  template<>\
  struct json_impl < TYPE , void >\
  {\
    struct json_helper : public TYPE\
    {\
      inline void read_(reader& rd)\
      {\
        auto& fields = this_field_list();\
        if (rd.expect('{') == false){ rd.error("read object must start with {!"); }\
          rd.next();\
        if (rd.expect('}'))\
          return;\
        auto mber = rd.peek();\
        do\
        {\
          if (mber.type != token::t_string){ rd.error("object key must be string"); }\
            rd.next();\
          if (rd.expect(':') == false){ rd.error("invalid json document!"); }\
            rd.next();\
          if (read_members(rd, &fields[0], mber.str, 0,__VA_ARGS__) == 0)\
          {\
            skip(rd);\
          }\
          if (rd.expect('}'))\
          {\
            rd.next();\
            return;\
          }\
          else if (rd.expect(','))\
          {\
            rd.next();\
            mber = rd.peek();\
            continue;\
          }\
          rd.error("invalid json document!");\
        } while (true);\
      }\
    template<typename write_ty>\
    inline void write_(write_ty& wt) const\
    {\
      auto& fields = this_field_list();\
      wt.putc('{');\
      ::ajson::write_members(wt, &fields[0], 0,__VA_ARGS__);\
      wt.putc('}');\
    }\
  };\
  static inline detail::field_list& this_field_list()\
  {\
    static auto fields = detail::split_fields(STRINGFY_LIST(__VA_ARGS__));\
    return fields;\
  }\
  static inline void read(reader& rd, TYPE& v)\
  {\
    reinterpret_cast<json_helper &>(v).read_(rd);\
  }\
  template<typename write_ty>\
  static inline void write(write_ty& wt, TYPE const& v)\
  {\
    reinterpret_cast<json_helper const &>(v).write_(wt);\
  }\
};\
}


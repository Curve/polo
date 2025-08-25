#include <boost/ut.hpp>
#include <polo/polo.hpp>

using namespace boost::ut;
using namespace boost::ut::bdd;

struct base
{
    static inline bool copy_ctor{false};
    static inline bool dtor{false};

  public:
    base() = default;

  public:
    base(const base &)
    {
        copy_ctor = true;
    }

  public:
    virtual ~base()
    {
        dtor = true;
    }

  public:
    [[nodiscard]] virtual bool is_base() const
    {
        return true;
    }
};

struct derived : base
{
    static inline bool copy_ctor{false};
    static inline bool dtor{false};

  public:
    derived() = default;

  public:
    derived(const derived &)
    {
        copy_ctor = true;
    }

  public:
    ~derived() override
    {
        dtor = true;
    }

  public:
    [[nodiscard]] bool is_base() const override
    {
        return false;
    }
};
// NOLINTNEXTLINE
suite<"basic"> basic_suite = []()
{
    {
        auto b = cr::polo<base>{};
        auto c = b;

        expect(b->is_base());
        expect(c->is_base());
    }

    expect(base::copy_ctor);
    expect(base::dtor);

    {
        auto d = cr::polo<base>{derived{}};
        auto c = d;

        expect(not d->is_base());
        expect(not c->is_base());
    }

    expect(derived::copy_ctor);
    expect(derived::dtor);

    auto b = cr::polo<base>{};
    auto d = cr::polo<base>{derived{}};

    expect(b->is_base());

    b = d;

    expect(not b.valueless_after_move());
    expect(not d.valueless_after_move());

    expect(not b->is_base());

    b = std::move(d);

    expect(not b.valueless_after_move());
    expect(d.valueless_after_move());

    expect(not b->is_base());
};


#include <string>
#include <algorithm>

namespace ds
{
    template<typename Type, int ValueCount>
    class b_tree
    {
        static_assert(ValueCount >= 1, "Wrong record per node in B+-Tree");
    public:
        explicit b_tree(bool unique = false);
        bool insert(Type);
        bool search(Type);
        bool remove(Type);
        bool update(Type);

        size_t size() const;
    private:

        struct tree_node
        {
            virtual ~tree_node() {}
            virtual bool contains(Type value) const = 0;
            virtual inline bool is_list() const = 0;
        };

        struct tree_internal_node
            : tree_node
        {
            virtual bool is_list() const override { return false; }
            virtual bool contains(Type value) const override;
            tree_node* get_next_level_ptr(Type value);

            tree_node* parent_node;
            Type values[ValueCount];
            size_t count;
            tree_node* pointers[ValueCount + 1];
        };

        struct tree_list_node
            : tree_node
        {
            virtual bool is_list() const override { return true; }
            virtual bool contains(Type value) const override;
            tree_node* parent_node;
            Type values[ValueCount];
            size_t count;
        };

        tree_list_node* find_list(tree_node* node, Type value);

        tree_node* root_;
        size_t size_;
    };

    template <typename Type, int ValueCount>
    b_tree<Type, ValueCount>::b_tree(bool unique)
        : root_(nullptr)
        , size_(0)
    {
        
    }

    template <typename Type, int ValueCount>
    bool b_tree<Type, ValueCount>::insert(Type value)
    {
        if(root_ == nullptr)
        {
            auto new_node = new tree_list_node();
            new_node->values[0] = value;
            new_node->count = 1;
            root_ = new_node;
        }
        else
        {
            auto list_node = find_list(root_, value);
            if(!list_node->contains(value))
            {
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    template <typename Type, int ValueCount>
    size_t b_tree<Type, ValueCount>::size() const
    {
        return size_;
    }

    template <typename Type, int ValueCount>
    bool b_tree<Type, ValueCount>::tree_internal_node::contains(Type value) const
    {
        return std::find_if(&values[0], &values[count], [value](Type x) -> bool {return value == x; }) != &values[count];
    }

    template <typename Type, int ValueCount>
    typename b_tree<Type, ValueCount>::tree_node* b_tree<Type, ValueCount>::tree_internal_node::get_next_level_ptr(Type value)
    {
        if(values[count - 1] < value)
        {
            return pointers[count];
        }

        size_t i = 0;
        while (values[i] < value) ++i;
        
        return pointers[i];
    }

    template <typename Type, int ValueCount>
    bool b_tree<Type, ValueCount>::tree_list_node::contains(Type value) const
    {
        return std::find_if(&values[0], &values[count], [value](Type x) -> bool { return value == x; }) != &values[count];
    }

    template <typename Type, int ValueCount>
    typename b_tree<Type, ValueCount>::tree_list_node* b_tree<Type, ValueCount>::find_list(tree_node* node, Type value)
    {
        if(node->is_list())
        {
            return static_cast<tree_list_node*>(node);
        }

        auto internal_node_ptr = static_cast<tree_internal_node*>(node);

        return find_list(internal_node_ptr->get_next_level_ptr(value), value);
    }
}


int main()
{
    ds::b_tree<int, 1> tree(false);
    tree.insert(10);
}
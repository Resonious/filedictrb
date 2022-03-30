# Filedictrb

Ruby wrapper for [filedict](https://github.com/Resonious/filedict).

The idea is the provide an interface that behaves exactly like a `Hash<Set<String>>`, but the data is bound to a file. The file is memory-mapped, and so it is shared across processes, and updates are flushed automatically by the operating system.

As of writing, this gem is still brand new, not production-ready. I'm pretty sure you'll get a segfault if you use non-string keys or values.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'filedictrb'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install filedictrb

## Usage

You can use `Filedict::Hash` almost just like a regular `Hash`.

```ruby
dict = Filedict::Hash.new('path/to/data/file')
dict['key'].add 'value'

dict['key'] # should equal Filedict::Set['value']

# if another process comes along and appends 'value 2', then ...
dict['key'] # should equal Filedict::Set['value', 'value 2']
```

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and the created tag, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/[USERNAME]/filedictrb.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

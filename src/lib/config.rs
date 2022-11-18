use super::types::*;
use clap::Parser;
use clap::ValueEnum;

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, ValueEnum, Debug)]
pub enum OutputGroup {
    Og1b,
    Og1d,
    Og1h,

    Og2d,
    Og2h,

    Og4d,
    Og4h,

    Og8d,
    Og8h,

    OgChar,
    OgRaw,
}

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, ValueEnum, Debug)]
pub enum Endianess {
    Big,
    Litte,
    Native,
}

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
pub struct Config {
    #[arg(short, long, help = "List of addresses")]
    address: Vec<Address>,
    #[arg(short, long, default_value_t = 0, help = "The base address")]
    base: Address,

    #[arg(long, default_value_t = 0, help = "The start address")]
    start: Address,

    #[arg(long, default_value_t = Address::MAX, help = "The end address")]
    end: Address,

    #[arg(long, short, value_enum)]
    endianes: Endianess,

    #[arg(long, short, value_enum)]
    group: OutputGroup,

    #[arg(long, help = "Disable address output")]
    no_addr: bool,

    #[arg(long, default_value_t = String::from(""), help = "Output prefix")]
    prefix: String,

    #[arg(long, default_value_t = String::from(" "), help = "Output separator")]
    separator: String,

    #[arg(
        long,
        short,
        default_value_t = 16,
        help = "How many bytes to display in each row"
    )]
    rowlen: usize,

    #[arg(long, default_value = "", help = "Set highlight escape code")]
    highlight: String,

    #[arg(long, default_value = "", help = "Set unhighlight escape code")]
    unhighlight: String,

    path: Option<String>,
}

impl Config {
    pub fn get_input(&self) -> Result<Box<dyn std::io::Read>, std::io::Error> {
        match &self.path {
            Some(path) => std::fs::File::open(path).map(|f| Box::new(f) as Box<dyn std::io::Read>),
            None => Ok(Box::new(std::io::stdin())),
        }
    }

    pub fn get_output(&self) -> Result<Box<dyn std::io::Write>, std::io::Error> {
        Ok(Box::new(std::io::stdout()))
    }
}

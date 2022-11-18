use thiserror::Error;

#[derive(Error, Debug)]
pub enum Error {
    #[error("Bad format operator")]
    BadFormat,
    #[error("File not found!")]
    FileNotFound,
}

-- MySQL Script generated by MySQL Workbench
-- Mon Apr 16 19:54:03 2018
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `mydb`.`smartgrid`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`smartgrid` (
  `Va` VARCHAR(12) NULL,
  `Vb` VARCHAR(12) NULL,
  `Vc` VARCHAR(12) NULL,
  `Ia` VARCHAR(12) NULL,
  `Ib` VARCHAR(12) NULL,
  `Ic` VARCHAR(12) NULL,
  `Total Power` VARCHAR(12) NULL,
  `Total Fundamental Power` VARCHAR(12) NULL,
  `Phase A Power` VARCHAR(12) NULL,
  `Phase B Power` VARCHAR(12) NULL,
  `Phase C Power` VARCHAR(12) NULL,
  `Reactive Power` VARCHAR(12) NULL,
  `Phase A Reactive Power` VARCHAR(12) NULL,
  `Phase B Reactive Power` VARCHAR(12) NULL,
  `Phase C Reactive Power` VARCHAR(12) NULL,
  `Consumed Power` VARCHAR(12) NULL,
  `Sold Power` VARCHAR(12) NULL)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
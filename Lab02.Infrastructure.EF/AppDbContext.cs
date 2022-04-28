﻿using System.Collections.Generic;
using System.Data.Entity;
using Lab02.Infrastructure.EF.Entities;

namespace Lab02.Infrastructure.EF
{
    class AppDbContextInitializer : DropCreateDatabaseAlways<AppDbContext>
    {
        protected override void Seed(AppDbContext dbContext)
        {
            dbContext.Operators.AddRange(new List<OperatorEntity>
            {
                new OperatorEntity
                {
                    Code = 29
                },
                new OperatorEntity
                {
                    Code = 44
                },
                new OperatorEntity
                {
                    Code = 33
                },
            });

            dbContext.SaveChanges();
        }
    }
    
    public class AppDbContext : DbContext
    {
        public DbSet<OperatorEntity> Operators { get; set; }
        public DbSet<RecordEntity> Records { get; set; }
        
        public AppDbContext() : base("DictionaryDb")
        {
            // Database.SetInitializer(new AppDbContextInitializer());
        }
    }
}
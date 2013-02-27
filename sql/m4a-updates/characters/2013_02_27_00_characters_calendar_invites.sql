alter table calendar_invites add column type enum('normal','signup') not null default 'normal' after rank;

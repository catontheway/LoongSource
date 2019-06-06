--��ҽű�
--��һ�����ߺ͵�һ�ν�����Ϸ����

--������ʱװ��װ
table_1stOnline_man = { {9000001, 9000002, 9000003, 9000004},
			{9000101, 9000102, 9000103, 9000104},
			{9000201, 9000202, 9000203, 9000204}}

--����Ůʱװ��װ
table_1stOnline_woman = {{9000011, 9000012, 9000013, 9000014},
			 {9000111, 9000112, 9000113, 9000114},
			 {9000211, 9000212, 9000213, 9000214}}


--��ҵ�һ������
function x_RoleFirstOnline(roleid)
	role.AddSkill(roleid, 9001001)  --����ҿɵ������ļ���
	role.AddQuest(roleid, 1)        --����ҳ�ʼ����
end

--��ҵ�һ�ν�����Ϸ����
function x_RoleFirstIntoWorld(mapid, instanceid, roleid)
	local sex = role.GetRoleSex(roleid)
	if(sex==nil) then return end

	local dressid = role.GetRoleDressid(roleid)
	if(dressid ==nil) then return else dressid = dressid + 1 end

	local table_1stOnline = {}
	if sex == 0 then
		table_1stOnline = table_1stOnline_woman --Ůʱװ
	elseif sex == 1 then
		table_1stOnline = table_1stOnline_man   --��ʱװ
	end

	--����ʱװ��װ����
	for i=1,4 do
		serial_high, serial_low, equip_pos = role.AddRoleItem(mapid, instanceid, roleid, table_1stOnline[dressid][i], 1, 0, -1, 380)
		if serial_high and serial_low and equip_pos then
			role.RoleEquip(mapid, instanceid, roleid, serial_high, serial_low, equip_pos)
		end
	end

	-- ��һ�ν�����Ϸ�������2������ҩ��2������ҩ
	local n64ID1, n64ID2 = role.AddRoleItem(mapid, instanceid, roleid, 1000003, 2, -1, 0, 380)
	local n64ID3, n64ID4 = role.AddRoleItem(mapid, instanceid, roleid, 1000103, 2, -1, 0, 380)
	role.VirginOnlineSendClient(roleid, n64ID1, n64ID2, n64ID3, n64ID4)

	-- �ڴν�����Ϸ����Ҷ�ȡ�񼣵ļ���
	role.AddSkill(roleid, 9001701)
end

-- ���ÿ�ν�����Ϸ����
function x_RoleIntoWorld(MapID, Instanceid, RoleID)

--------------------------------------�����---------------------------------------
	-- �鿴��ҵİ����Ƿ��ȡ����
	local guildID = 0
	guildID = guild.GetRoleGuildID(RoleID)
	local GuildPos
	GuildPos  = guild.GetRoleGuildPos(guildID, RoleID);
	if GuildPos ~= nil then
		local CapGuildNum = GetGuildCaptureNum(guildID)
		if CapGuildNum > 0 then
			local BuffId = GetBuffIDbyGodMiracleNum(CapGuildNum)
			unit.AddBuff(MapID, Instanceid, RoleID, BuffId, RoleID)
		end
	end

	-- ���Ӷ�ȡ�񼣵ļ���
	--if role.IsLearnedSkill(RoleID, 90017) == nil then
		role.AddSkill(RoleID, 9001701)
	--end
--------------------------------------��END---------------------------------------

--------------------------------------�����������---------------------------------------
    local curyear = tonumber(os.date("%Y"))
	if curyear ~= role.GetRoleScriptData(RoleID, 1, RoleDataType["YinYuanJie_Year"]) then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["YinYuanJie"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LeiFengMao"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["YinYuanJie_Year"], curyear)
    end
--------------------------------------�������END---------------------------------------

--------------------------------------�������---------------------------------------
	local level = role.GetRoleLevel(MapID, Instanceid, RoleID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
	if level >= 30 and k < 99 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 3101)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
--------------------------------------����END---------------------------------------

--------------------------------------�������---------------------------------------
	if  FeiSheng_trigger == 1 and level >= 80 and role.GetSoarValue(RoleID) == 0 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 3130)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
--------------------------------------�������---------------------------------------

---------------------------------------��������------------------------------------

	local a = role.IsLearnedSkill(RoleID,11201)
	local b = role.IsLearnedSkill(RoleID,11217)
	local level = role.GetRoleLevel(MapID, Instanceid, RoleID)
	if level >= 50 then
		if a == nil and b == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3516)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end

---------------------------------------��������------------------------------------

--------------------------------------�������--------------------------------------
    local level = role.GetRoleLevel(MapID, Instanceid, RoleID)

	if level >= 5 then
		role.OpenTrain(RoleID, 78)
	end
	if level >= 10 then
		role.OpenTrain(RoleID, 79)
	end
	if level >= 15 then
		role.OpenTrain(RoleID, 12)
	end
	if level >= 20 then
		role.OpenTrain(RoleID, 80)
		role.OpenTrain(RoleID, 39)
		role.OpenTrain(RoleID, 47)
	end
	if level >= 25 then
		role.OpenTrain(RoleID, 1)
		role.OpenTrain(RoleID, 7)
	end
	if level >= 30 then
		role.OpenTrain(RoleID, 30)
		role.OpenTrain(RoleID, 48)
	end
	if level >= 35 then
		role.OpenTrain(RoleID, 63)
	end
	if level >= 40 then
		role.OpenTrain(RoleID, 37)
		role.OpenTrain(RoleID, 38)
		role.OpenTrain(RoleID, 49)
	end
	if level >= 45 then
		role.OpenTrain(RoleID, 18)
		role.OpenTrain(RoleID, 23)
		role.OpenTrain(RoleID, 28)
	end
	if level >= 50 then
		role.OpenTrain(RoleID, 50)
		--role.OpenTrain(RoleID, 71)
	end
	if level >= 55 then
		role.OpenTrain(RoleID, 51)
		role.OpenTrain(RoleID, 73)
	end
	if level >= 60 then
		role.OpenTrain(RoleID, 52)
	end
	if level >= 65 then
		role.OpenTrain(RoleID, 53)
		role.OpenTrain(RoleID, 74)
	end
	if level >= 70 then
		role.OpenTrain(RoleID, 54)
	end
	if level >= 75 then
		role.OpenTrain(RoleID, 55)
		role.OpenTrain(RoleID, 75)
	end
	if level >= 80 then
		role.OpenTrain(RoleID, 56)
	end
	if level >= 85 then
		role.OpenTrain(RoleID, 59)
		role.OpenTrain(RoleID, 76)
    end
	if level >= 90 then
		role.OpenTrain(RoleID, 60)
    end
	if level >= 95 then
		role.OpenTrain(RoleID, 61)
		role.OpenTrain(RoleID, 77)
    end
	if level >= 100 then
		role.OpenTrain(RoleID, 62)
	end
--------------------------------------����END---------------------------------------

end

-- ��ҵȼ��ı�
function x_RoleLevelChange(MapID, InstanceID, RoleID)

-------------------------------------ʦͽ���--------------------------------------------------

	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local MasterID = role.GetMasterID(MapID, InstanceID, RoleID)
	local bHaveGraduate = role.GetRoleScriptData(RoleID, 1, RoleDataType["HaveGraduate"])	-- �Ƿ��Ѿ���ʦ

	if MasterID ~= 4294967295  and level < 50 and bHaveGraduate ~= 1 then	-- ��ʦ����δ��ʦ
		role.LevelUpAddJingWuPoint(MapID, InstanceID, RoleID)		-- Ϊʦ�����Ӿ������

		-- �ȼ�����10����ͽ����������Ϊʦ��������Ӧ����
		if level > 10 then
			role.LevelUpAddBonus(MapID, InstanceID, RoleID)
		end

		-- �ȼ�Ϊ10������������Ϊʹ�þ������������ȫ���㲥
		local UseJingWuPoint = role.GetRoleScriptData(RoleID, 1, RoleDataType["UseJingWuPoint"])
		if math.fmod(level, 10) == 0 and UseJingWuPoint == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 860)
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 9, level)
			msg.AddMsgEvent(MsgID, 2, MasterID)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end

	if MasterID ~= 4294967295 and level == 50 and bHaveGraduate ~= 1 then	-- ��ʦ
		role.GraduateNotice(MapID, InstanceID, RoleID)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["GraduateReward"], 1)	-- ����δ��ȡ��ʦ����
		role.SetRoleScriptData(RoleID, 1, RoleDataType["HaveGraduate"], 1)		-- ����Ϊ�ѳ�ʦ
	end

	-- ����ʹ�þ����������
	role.SetRoleScriptData(RoleID, 1, RoleDataType["UseJingWuPoint"], 0)

---------------------------------------ʦͽEND----------------------------------------------------------


---------------------------------------��������----------------------------------------------------------

	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level == 50 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 3516)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

---------------------------------------����END----------------------------------------------------------



---------------------------------------�������㲥���-----------------------------------------------
	--math.randomseed(os.time());						--�����������
	local n_RoleLv = role.GetRoleLevel(MapID, InstanceID, RoleID);		--��ȡ���������ĵȼ�
	if ( n_RoleLv >= 80 ) then						--�ж����������ȼ��Ƿ�ﵽ80������δ�ﵽ����㲥
		if ( math.mod(n_RoleLv,5) == 0 ) then				--�ж����������ĵȼ��Ƿ�ɱ�5��������ʱ����д������Թ㲥
			local r = math.random(1,100);				--ȡ��������������������Թ㲥�����ѡȡһ�����в���
			if ( r<26 ) then
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102,100009);
				msg.AddMsgEvent(MsgID,2,RoleID);
				msg.AddMsgEvent(MsgID,9,n_RoleLv);
				msg.DispatchBroadcast(MsgID,-1,-1,-1);
			elseif ( r<51 ) then
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102,100010);
				msg.AddMsgEvent(MsgID,2,RoleID);
				msg.AddMsgEvent(MsgID,9,n_RoleLv);
				msg.DispatchBroadcast(MsgID,-1,-1,-1);
			elseif ( r<76 ) then
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102,100011);
				msg.AddMsgEvent(MsgID,2,RoleID);
				msg.AddMsgEvent(MsgID,9,n_RoleLv);
				msg.DispatchBroadcast(MsgID,-1,-1,-1);
			else
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102,100012);
				msg.AddMsgEvent(MsgID,2,RoleID);
				msg.AddMsgEvent(MsgID,9,n_RoleLv);
				msg.DispatchBroadcast(MsgID,-1,-1,-1);
			end
		else
			if ( n_RoleLv>=90 ) then				--����ҵȼ�Ϊ80-90������60%���ʴ����㲥������5%����Ϊȫ���㲥������ҵȼ�Ϊ90�����ϣ�����80%���ʴ����㲥������10%����Ϊȫ���㲥��
				local r1 = math.random(1,100);
				if ( r1<81 ) then
					local r2 = math.random(1,100);
					if ( r2<26 ) then					--���ѡ��һ��С������ƹ㲥����
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100013);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					elseif ( r2<51 ) then
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100014);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					elseif ( r2<76 ) then
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100015);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					else
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100016);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					end
				end
			else
				local r1 = math.random(1,100);
				if ( r1<61 ) then
					local r2 = math.random(1,100);
					if ( r2<26 ) then					--���ѡ��һ��С������ƹ㲥����
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100013);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					elseif ( r2<51 ) then
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100014);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					elseif ( r2<76 ) then
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100015);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<6 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					else
						local MsgID = msg.BeginMsgEvent();
						msg.AddMsgEvent(MsgID,102,100016);
						msg.AddMsgEvent(MsgID,2,RoleID);
						msg.AddMsgEvent(MsgID,9,n_RoleLv);
						if ( r1<11 ) then
							msg.DispatchBroadcast(MsgID,-1,-1,-1);
						else
							msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
						end
					end
				end
			end
		end
	end
----------------------------------�������㲥END------------------------------------------------


---------------------------------------�������---------------------------------------------------------

	if level == 5 then
		role.OpenTrain(RoleID, 78)
	elseif level == 10 then
		role.OpenTrain(RoleID, 79)
	elseif level == 15 then
		role.OpenTrain(RoleID, 12)
	elseif level == 20 then
		role.OpenTrain(RoleID, 80)
		role.OpenTrain(RoleID, 39)
		role.OpenTrain(RoleID, 47)
	elseif level == 25 then
		role.OpenTrain(RoleID, 1)
		role.OpenTrain(RoleID, 7)
	elseif level == 30 then
		role.OpenTrain(RoleID, 30)
		role.OpenTrain(RoleID, 48)
	elseif level == 35 then
		role.OpenTrain(RoleID, 63)
	elseif level == 40 then
		role.OpenTrain(RoleID, 37)
		role.OpenTrain(RoleID, 38)
		role.OpenTrain(RoleID, 49)
	elseif level == 45 then
		role.OpenTrain(RoleID, 18)
		role.OpenTrain(RoleID, 23)
		role.OpenTrain(RoleID, 28)
	elseif level == 50 then
		role.OpenTrain(RoleID, 50)
		--role.OpenTrain(RoleID, 71)
	elseif level == 55 then
		role.OpenTrain(RoleID, 51)
		role.OpenTrain(RoleID, 73)
	elseif level == 60 then
		role.OpenTrain(RoleID, 52)
	elseif level == 65 then
		role.OpenTrain(RoleID, 53)
		role.OpenTrain(RoleID, 74)
	elseif level == 70 then
		role.OpenTrain(RoleID, 54)
	elseif level == 75 then
		role.OpenTrain(RoleID, 55)
		role.OpenTrain(RoleID, 75)
	elseif level == 80 then
		role.OpenTrain(RoleID, 56)
	elseif level == 85 then
		role.OpenTrain(RoleID, 59)
		role.OpenTrain(RoleID, 76)
	elseif level == 90 then
		role.OpenTrain(RoleID, 60)
	elseif level == 95 then
		role.OpenTrain(RoleID, 61)
		role.OpenTrain(RoleID, 77)
	elseif level == 100 then
		role.OpenTrain(RoleID, 62)
	end

---------------------------------------����END---------------------------------------------------------

---------------------------------------�������--------------------------------------------------------

	if Salary_NPC_On==1 then

		local Have_FreePoint = role.GetRoleScriptData(RoleID, 1, RoleDataType["Have_FreePoint"])

		if level == 35 and Have_FreePoint==0 then
			role.AddRoleGiftPoints(MapID, InstanceID, RoleID, 3000)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Have_FreePoint"], 1)
		end

	end
---------------------------------------����END---------------------------------------------------------

end

-- ��Ұ�ʦ
function x_RoleHandleMaster(MasterID, ApprenticeID)
	local MsgID = msg.BeginMsgEvent()		-- ȫ�������...
	msg.AddMsgEvent(MsgID, 100, 2330)
	msg.AddMsgEvent(MsgID, 2, MasterID)
	msg.AddMsgEvent(MsgID, 2, ApprenticeID)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- �������
function x_RoleFAbaoLevelup(MapID, InstanceID, RoleID, FabaoID, Type, FBVal)
	if Type == 0 then
	    if FBVal >= 11 and FBVal < 21 then
		    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
			if k % 10 < 2 then
			    k = math.floor(k/10) * 10 + 2
			end
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"], k)
		elseif FBVal >= 21 and FBVal < 31 then
			local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
			if k % 10 < 3 then
			    k = math.floor(k/10) * 10 + 3
			end
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"], k)
		elseif FBVal >= 31 and FBVal < 41 then
			local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
			if k % 10 < 4 then
			    k = math.floor(k/10) * 10 + 4
			end
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"], k)
		elseif FBVal >= 41 then
		    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
			if k % 10 < 5 then
			    k = math.floor(k/10) * 10 + 5
			end
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"], k)
		end
	elseif Type == 1 or Type == 2 then
	    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
		if k < 10 then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"], 10 + k)
		end
	end
end

function x_RoleLianQi(MapID, InstanceID, RoleID, ItemID)
    local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"])
	local k = b % 1000
	if k <= 200 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"], b + 10)
	end
end


function x_RoleRongZhuLevelup(MapID, InstanceID, RoleID, ItemID, Level)
    local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"])
	local k = b % 1000
	if Level == 20 and b < 4000 then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"], 4000+k)
	elseif Level >= 10 and b < 3000 then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"], 3000+k)
	elseif Level >= 5 and b < 2000 then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"], 2000+k)
	elseif Level >= 1 and b < 1000 then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiSheng_shilian"], 1000+k)
	end
end

--ע�ắ��
aux.RegisterRoleEvent(1, "x_RoleFirstOnline")
aux.RegisterRoleEvent(2, "x_RoleIntoWorld")
aux.RegisterRoleEvent(3, "x_RoleFirstIntoWorld")
aux.RegisterRoleEvent(6, "x_RoleLevelChange")
aux.RegisterRoleEvent(27, "x_RoleHandleMaster")
aux.RegisterRoleEvent(35, "x_RoleFAbaoLevelup")
aux.RegisterRoleEvent(40, "x_RoleLianQi")
aux.RegisterRoleEvent(41, "x_RoleRongZhuLevelup")

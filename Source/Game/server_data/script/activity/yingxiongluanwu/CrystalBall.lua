--ˮ����TypeID��buff��Ӧ��
--Crystal_Ball = {}
--Crystal_Ball[1520001] = 2013301
--Crystal_Ball[1520002] = 2013401
--Crystal_Ball[1520003] = 2013501
--Crystal_Ball[1520004] = 2013601
--Crystal_Ball[1520005] = 2013701
--Crystal_Ball[1520006] = 2013801
--Crystal_Ball[1520007] = 2013901
--[[Crystal_Ball[1520008] = 2014001
Crystal_Ball[1520009] = 2014101
Crystal_Ball[1520010] = 2014201
Crystal_Ball[1520011] = 2014301
Crystal_Ball[1520012] = 2014401
Crystal_Ball[1520013] = 2014501
Crystal_Ball[1520014] = 2014601
Crystal_Ball[1520015] = 2014701
Crystal_Ball[1520016] = 2014801
Crystal_Ball[1520017] = 2014901
Crystal_Ball[1520018] = 2015001
Crystal_Ball[1520019] = 2015101]]

--[[��ɱˮ�����������һ���
local function IncScore(MapID, InstanceID, KillerID, Score)
	if MapID == 2983744463 then
		if InstanceID == 1 then
			Act14_RoleData[KillerID].Score = Act14_RoleData[KillerID].Score + Score
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	153)								--������
			msg.AddMsgEvent(MsgID, 14,	Act14_RoleData[KillerID].Score)		--����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end

		if InstanceID == 2 then
			Act15_RoleData[KillerID].Score = Act15_RoleData[KillerID].Score + Score
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	154)								--������
			msg.AddMsgEvent(MsgID, 14,	Act15_RoleData[KillerID].Score)		--����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end

		if InstanceID == 3 then
			Act16_RoleData[KillerID].Score = Act16_RoleData[KillerID].Score + Score
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	155)								--Ӣ����
			msg.AddMsgEvent(MsgID, 14,	Act16_RoleData[KillerID].Score)		--����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end

		if InstanceID == 4 then
			Act17_RoleData[KillerID].Score = Act17_RoleData[KillerID].Score + Score
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	156)								--������
			msg.AddMsgEvent(MsgID, 14,	Act17_RoleData[KillerID].Score)		--����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end
	end

	if MapID == 2983743695 then
		local KillerTeamKey = role.GetRoleScriptData(KillerID, 1, RoleDataType["Act18_TeamKey"])
		if InstanceID == 1 then
		    Act18_TeamData[KillerTeamKey].Score = Act18_TeamData[KillerTeamKey].Score + Score
            SendScore2Teamate(153, Act18_TeamData[KillerTeamKey].Score, Act18_TeamRoleData[KillerTeamKey])
		end

		if InstanceID == 2 then
            Act19_TeamData[KillerTeamKey].Score = Act19_TeamData[KillerTeamKey].Score + Score
            SendScore2Teamate(154, Act19_TeamData[KillerTeamKey].Score, Act19_TeamRoleData[KillerTeamKey])
		end

		if InstanceID == 3 then
            Act20_TeamData[KillerTeamKey].Score = Act20_TeamData[KillerTeamKey].Score + Score
            SendScore2Teamate(155, Act20_TeamData[KillerTeamKey].Score, Act20_TeamRoleData[KillerTeamKey])
		end

		if InstanceID == 4 then
            Act21_TeamData[KillerTeamKey].Score = Act21_TeamData[KillerTeamKey].Score + Score
            SendScore2Teamate(156, Act21_TeamData[KillerTeamKey].Score, Act21_TeamRoleData[KillerTeamKey])
		end
	end
end]]

--[[ˢ�»�о������
function RefreshCrystalCount(MapID, InstanceID, CreatureID)
    local PosIndex = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)
    if MapID == 2983744463 then
		if InstanceID == 1 then
		    m21_SpawnPos[PosIndex].Crystal40 = false
		    Act14_CrystalNum = Act14_CrystalNum - 1
		end

		if InstanceID == 2 then
		    m21_SpawnPos[PosIndex].Crystal60 = false
		    Act15_CrystalNum = Act15_CrystalNum - 1
		end

		if InstanceID == 3 then
		    m21_SpawnPos[PosIndex].Crystal80 = false
		    Act16_CrystalNum = Act16_CrystalNum - 1
		end

		if InstanceID == 4 then
		    m21_SpawnPos[PosIndex].Crystal100 = false
		    Act17_CrystalNum = Act17_CrystalNum - 1
		end
	end

	if MapID == 2983743695 then
	    if InstanceID == 1 then
	        m22_SpawnPos[PosIndex].Crystal40 = false
		    Act18_CrystalNum = Act18_CrystalNum - 1
	    end

	    if InstanceID == 2 then
	        m22_SpawnPos[PosIndex].Crystal60 = false
		    Act19_CrystalNum = Act19_CrystalNum - 1
	    end

	    if InstanceID == 3 then
	        m22_SpawnPos[PosIndex].Crystal80 = false
		    Act20_CrystalNum = Act20_CrystalNum - 1
	    end

	    if InstanceID == 4 then
	        m22_SpawnPos[PosIndex].Crystal100 = false
		    Act21_CrystalNum = Act21_CrystalNum - 1
	    end
	end
end]]

--[[��������
function c1520001_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013301, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520001, 4, "c1520001_OnDie")]]

--[[�ָ�����
function c1520002_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013401, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520002, 4, "c1520002_OnDie")

--���پ���
function c1520003_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013501, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520003, 4, "c1520003_OnDie")

--��������--��������
function c1520004_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013601, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520004, 4, "c1520004_OnDie")

--���ھ���--���徧��
function c1520005_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013701, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520005, 4, "c1520005_OnDie")

--��ӯ����--�翹����
function c1520006_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013801, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520006, 4, "c1520006_OnDie")

--���˾���--���߾���
function c1520007_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2013901, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	--IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520007, 4, "c1520007_OnDie")

--��������
function c1520008_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2014001, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520008, 4, "c1520008_OnDie")

--�翹����
function c1520009_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2014101, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520009, 4, "c1520009_OnDie")

--�޵о���
function c1520010_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	unit.AddBuff(MapID, InstanceID, KillerID, 2014201, KillerID)
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520010, 4, "c1520010_OnDie")

--��ɱ����
function c1520011_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014301, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520011, 4, "c1520011_OnDie")

--�ֶ�����
function c1520012_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014401, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520012, 4, "c1520012_OnDie")

--���Ӿ���
function c1520013_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014501, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520013, 4, "c1520013_OnDie")

--ǿ�徧��
function c1520014_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014601, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520014, 4, "c1520014_OnDie")

--��������
function c1520015_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014701, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520015, 4, "c1520015_OnDie")

--���Ծ���
function c1520016_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014801, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520016, 4, "c1520016_OnDie")

--��������
function c1520017_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2014901, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520017, 4, "c1520017_OnDie")

--������
function c1520018_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2015001, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520018, 4, "c1520018_OnDie")

--��������
function c1520019_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ȡ������Χ���
	local RoleIDTable = cre.GetAroundRole(MapID, InstanceID, CreatureID, 400, 200)

	for k in pairs(RoleIDTable) do
		if unit.IsDead(MapID, InstanceID, RoleIDTable[k]) == false then
			unit.AddBuff(MapID, InstanceID, RoleIDTable[k], 2015101, RoleIDTable[k])
		end
	end
	RefreshCrystalCount(MapID, InstanceID, CreatureID)
	--��û���+2
	IncScore(MapID, InstanceID, KillerID, 2)
end

aux.RegisterCreatureEvent(1520019, 4, "c1520019_OnDie")]]

--[[����Lv70
function c1520020_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��û���+5
	IncScore(MapID, InstanceID, KillerID, 5)
end

aux.RegisterCreatureEvent(1520020, 4, "c1520020_OnDie")

--����Lv90
function c1520021_OnDie(MapID, InstanceID, CreatureID,TypeID, KillerID)
	--��û���+5
	IncScore(MapID, InstanceID, KillerID, 5)
end

aux.RegisterCreatureEvent(1520021, 4, "c1520021_OnDie")

--����Lv110
function c1520022_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��û���+5
	IncScore(MapID, InstanceID, KillerID, 5)
end

aux.RegisterCreatureEvent(1520022, 4, "c1520022_OnDie")]]
















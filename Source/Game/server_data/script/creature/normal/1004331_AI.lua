--���ﺰ���ű�

-- �������ս������
function c1004331_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10048)
       end	
end

aux.RegisterCreatureEvent(1004331, 2, "c1004331_OnEnterCombat")

